/// \example
#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>

#include <concurrent/dispatcher.h>
#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

// ############################## messages
struct msg_a {
  friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                        const msg_a &p_msg) {
    p_out << "|msg a|i|" << p_msg.i;
    return p_out;
  }

  explicit msg_a(int16_t p_i = 0) : i(p_i) {}

  int16_t i = {0};
};

struct msg_b {
  friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                        const msg_b &p_msg) {
    p_out << "|msg b|i|" << p_msg.i;
    return p_out;
  }

  explicit msg_b(int32_t p_i = 0) : i(p_i) {}
  int32_t i = {0};
};

struct msg_c {
  friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                        const msg_c &p_msg) {
    p_out << "|msg c|d|" << p_msg.d;
    return p_out;
  }

  explicit msg_c(double p_d = 0.0)
      : d(p_d)

  {}

  double d = {0.0};
};

// ############################## publishers
struct publisher_1 {
public:
  concurrent::_bus::work_status operator()() {
    using namespace tenacitas;
    msg_a _msg(++i);
    concurrent_log_debug(logger::cerr::log, "P 1", _msg);

    concurrent::_bus::dispatcher_t<msg_a, logger::cerr::log>::publish(++i);
    return concurrent::_bus::work_status::dont_stop;
  }
  int16_t i = {10};
};

struct publisher_2 {
public:
  concurrent::_bus::work_status operator()() {
    using namespace tenacitas;
    i += 10;
    msg_a _msg_a(i);
    concurrent_log_debug(logger::cerr::log, "P 2", _msg_a);
    concurrent::_bus::dispatcher_t<msg_a, logger::cerr::log>::publish(_msg_a);

    d *= 2.5;
    msg_c _msg_c(d);
    concurrent_log_debug(logger::cerr::log, "P 2", _msg_c);
    concurrent::_bus::dispatcher_t<msg_c, logger::cerr::log>::publish(_msg_c);

    return concurrent::_bus::work_status::dont_stop;
  }
  int16_t i = {-100};
  double d = {1.0};
};

struct publisher_3 {
public:
  concurrent::_bus::work_status operator()() {
    using namespace tenacitas;
    i += 300;
    msg_b _msg_b(i);

    concurrent_log_debug(logger::cerr::log, "P 3", _msg_b);
    concurrent::_bus::dispatcher_t<msg_b, logger::cerr::log>::publish(_msg_b);

    return concurrent::_bus::work_status::dont_stop;
  }

  int32_t i = {5000};
};

// ############################## subscribers
struct subscriber_1 {
  concurrent::_bus::work_status operator()(msg_a &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 1", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
};

struct subscriber_2 {
  concurrent::_bus::work_status operator()(msg_a &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 2", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }

  concurrent::_bus::work_status operator()(msg_b &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 2", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
};

struct subscriber_3 {
  concurrent::_bus::work_status operator()(msg_b &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 3", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
};

struct subscriber_4 {
  concurrent::_bus::work_status operator()(msg_a &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }

  concurrent::_bus::work_status operator()(msg_b &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }

  concurrent::_bus::work_status operator()(msg_c &&p_msg) {
    using namespace tenacitas;
    concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
};

typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

// ############################## subscribers
struct dispatcher_000 {

  bool operator()() {
    using namespace tenacitas;
    sleeping_loop _sl1(std::chrono::milliseconds(1000), publisher_1(),
                       std::chrono::milliseconds(1000));
    sleeping_loop _sl2(std::chrono::milliseconds(2000), publisher_2(),
                       std::chrono::milliseconds(1000));
    sleeping_loop _sl3(std::chrono::milliseconds(1500), publisher_3(),
                       std::chrono::milliseconds(1000));
    using namespace tenacitas;
    bool _rc = true;
    std::chrono::milliseconds _work_timeout(500);
    concurrent::_bus::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a1", []() { return subscriber_1(); }, 2, _work_timeout);
    concurrent::_bus::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a2", subscriber_2(), _work_timeout);
    concurrent::_bus::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a3", subscriber_4(), _work_timeout);
    concurrent::_bus::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b1", subscriber_2(), _work_timeout);
    concurrent::_bus::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b2", subscriber_3(), _work_timeout);
    concurrent::_bus::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b3", subscriber_4(), _work_timeout);
    concurrent::_bus::dispatcher_t<msg_c, logger::cerr::log>::subscribe(
        "c1", subscriber_4(), _work_timeout);

    _sl1.run();
    _sl2.run();
    _sl3.run();

    concurrent_log_debug(logger::cerr::log, "------> starting to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_debug(logger::cerr::log, "------> waking up");
    return _rc;
  }

  static const std::string desc() {
    return "\n3 types of messages, 4 subscribers, 3 publishers."
           "\n\nMessage 'a' has an int16_t attribute; message 'b' has an "
           "int32_t "
           "attribute; message 'c' has a double attribute."

           "\n\nSubscriber 1 for message 'a', with two instances; subscriber 2 "
           "for "
           "messages 'a' and 'b'; subscriber 3 for message 'b'; "
           "subscriber 4 for messages 'a', 'b' and 'c'."

           "\n\nPublisher 1 publishes 'a', starting at 10, increasing 1 at "
           "each "
           "publishing; "
           "publisher 2 publishes 'a' stating at -100, and incrementing "
           "10, and 'c', starting at 1.0 and multiplying times 2.5; "
           "publisher 3 publishes 'b', stating at 5000, and incrementing 300."

           "\n\nA 'sleeping_loop' will call Publisher 1 to send messages at "
           "each "
           "1000ms; "
           "another 'sleeping_loop' will call Publisher 2 to send messages at "
           "each "
           "2000ms; "
           "and a third 'sleeping_loop' will call Publisher 3 to send messages "
           "at "
           "each 1500ms."

           "\n\nMain thread will sleep for 10 seconds, while publishers send "
           "messages.";
  }
  static const std::string name() { return "dispatcher_000"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<dispatcher_000>(argc, argv);
}