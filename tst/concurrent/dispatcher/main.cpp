/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

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

// ############################## subscribers
struct dispatcher_000 {
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
    concurrent::status operator()() {
      using namespace tenacitas;
      msg_a _msg(++i);
      concurrent_log_debug(logger::cerr::log, "P 1", _msg);

      concurrent::dispatcher_t<msg_a, logger::cerr::log>::publish(++i);
      return concurrent::status::dont_stop;
    }
    int16_t i = {10};
  };

  struct publisher_2 {
  public:
    concurrent::status operator()() {
      using namespace tenacitas;
      i += 10;
      msg_a _msg_a(i);
      concurrent_log_debug(logger::cerr::log, "P 2", _msg_a);
      concurrent::dispatcher_t<msg_a, logger::cerr::log>::publish(_msg_a);

      d *= 2.5;
      msg_c _msg_c(d);
      concurrent_log_debug(logger::cerr::log, "P 2", _msg_c);
      concurrent::dispatcher_t<msg_c, logger::cerr::log>::publish(_msg_c);

      return concurrent::status::dont_stop;
    }
    int16_t i = {-100};
    double d = {1.0};
  };

  struct publisher_3 {
  public:
    concurrent::status operator()() {
      using namespace tenacitas;
      i += 300;
      msg_b _msg_b(i);

      concurrent_log_debug(logger::cerr::log, "P 3", _msg_b);
      concurrent::dispatcher_t<msg_b, logger::cerr::log>::publish(_msg_b);

      return concurrent::status::dont_stop;
    }

    int32_t i = {5000};
  };

  // ############################## subscribers
  struct subscriber_1 {
    concurrent::status operator()(msg_a &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 1", p_msg);
      return concurrent::status::dont_stop;
    }
  };

  struct subscriber_2 {
    concurrent::status operator()(msg_a &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 2", p_msg);
      return concurrent::status::dont_stop;
    }

    concurrent::status operator()(msg_b &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 2", p_msg);
      return concurrent::status::dont_stop;
    }
  };

  struct subscriber_3 {
    concurrent::status operator()(msg_b &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 3", p_msg);
      return concurrent::status::dont_stop;
    }
  };

  struct subscriber_4 {
    concurrent::status operator()(msg_a &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
      return concurrent::status::dont_stop;
    }

    concurrent::status operator()(msg_b &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
      return concurrent::status::dont_stop;
    }

    concurrent::status operator()(msg_c &&p_msg) {
      using namespace tenacitas;
      concurrent_log_debug(logger::cerr::log, "S 4", p_msg);
      return concurrent::status::dont_stop;
    }
  };

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

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
    concurrent::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a1", []() { return subscriber_1(); }, 2, _work_timeout);
    concurrent::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a2", subscriber_2(), _work_timeout);
    concurrent::dispatcher_t<msg_a, logger::cerr::log>::subscribe(
        "a3", subscriber_4(), _work_timeout);
    concurrent::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b1", subscriber_2(), _work_timeout);
    concurrent::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b2", subscriber_3(), _work_timeout);
    concurrent::dispatcher_t<msg_b, logger::cerr::log>::subscribe(
        "b3", subscriber_4(), _work_timeout);
    concurrent::dispatcher_t<msg_c, logger::cerr::log>::subscribe(
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

// ############################## subscribers
struct dispatcher_001 {
  // ############################## messages
  struct request {
    friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                          const request &p_msg) {
      p_out << "|msg a|i|" << p_msg.i;
      return p_out;
    }

    explicit request(int16_t p_i = 0) : i(p_i) {}

    int32_t i = {0};
  };

  struct reply {
    friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                          const reply &p_msg) {
      p_out << "|msg a|i|" << p_msg.i;
      return p_out;
    }

    explicit reply(int16_t p_i = 0) : i(p_i) {}

    int32_t i = {0};
  };

  // ############################## processors
  struct requester {
    concurrent::status start() {
      using namespace tenacitas;
      using namespace tenacitas;
      typedef concurrent::dispatcher_t<request, logger::cerr::log> dispatcher;
      dispatcher::publish(request(time(nullptr)));
      return concurrent::status::dont_stop;
    }

    concurrent::status operator()(reply &&p_reply) {
      using namespace tenacitas;
      using namespace tenacitas;
      typedef concurrent::dispatcher_t<request, logger::cerr::log> dispatcher;
      concurrent_log_debug(logger::cerr::log, "reply ", p_reply);
      if (m_counter++ > 100) {
        concurrent_log_info(logger::cerr::log, "counter = ", m_counter,
                            ", stopping");
        dispatcher::publish(request(0));
        return concurrent::status::stop;
      }
      if ((p_reply.i % 2) == 0) {
        dispatcher::publish(request(5 * time(nullptr)));
      } else {
        dispatcher::publish(request(3 * time(nullptr)));
      }
      return concurrent::status::dont_stop;
    }

  private:
    int16_t m_counter = {0};
  };

  struct replier {
    concurrent::status operator()(request &&p_request) {
      using namespace tenacitas;
      using namespace tenacitas;
      typedef concurrent::dispatcher_t<reply, logger::cerr::log> dispatcher;
      concurrent_log_debug(logger::cerr::log, "request ", p_request);
      if (p_request.i == 0) {
        concurrent_log_info(logger::cerr::log, "stopping");
        return concurrent::status::stop;
      }
      if ((p_request.i % 2) == 0) {
        dispatcher::publish(reply(4 * time(nullptr)));
      } else {
        dispatcher::publish(reply(4 * time(nullptr)));
      }
      return concurrent::status::dont_stop;
    }
  };

  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<reply, logger::cerr::log> dispatcher_reply;
    typedef concurrent::dispatcher_t<request, logger::cerr::log>
        dispatcher_request;
    std::chrono::milliseconds _work_timeout(500);
    dispatcher_reply::subscribe("req", requester(), _work_timeout);
    dispatcher_request::subscribe("rep", replier(), _work_timeout);

    requester().start();

    concurrent_log_debug(logger::cerr::log, "------> starting to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_debug(logger::cerr::log, "------> waking up");

    return true;
  }

  static const std::string desc() {
    return "\nTesting asynchronous work collaboration.";
  }
  static const std::string name() { return "dispatcher_001"; }
};

struct dispatcher_002 {

  // ############################## messages
  struct request {
    friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                          const request &p_msg) {
      p_out << "|msg a|i|" << p_msg.i;
      return p_out;
    }

    explicit request(uint32_t p_i = 0) : i(p_i) {}

    uint32_t i = {0};
  };

  struct reply {
    friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                          const reply &p_msg) {
      p_out << "|msg a|i|" << p_msg.i;
      return p_out;
    }

    explicit reply(uint32_t p_i = 0) : i(p_i) {}

    uint32_t i = {0};
  };

  // ############################## processors
  struct requester {
    requester(std::condition_variable *p_cond, std::mutex *p_mutex)
        : m_cond(p_cond), m_mutex(p_mutex) {}

    concurrent::status operator()(reply &&p_reply) {
      using namespace tenacitas;
      using namespace tenacitas;
      typedef concurrent::dispatcher_t<request, logger::cerr::log> dispatcher;
      concurrent_log_info(logger::cerr::log, "reply ", p_reply);
      if (m_counter++ > 100) {
        concurrent_log_info(logger::cerr::log, "counter = ", m_counter,
                            ", stopping");
        dispatcher::publish(request(0));
        concurrent_log_info(logger::cerr::log, "sleeping...");
        std::this_thread::sleep_for(std::chrono::seconds(8));
        concurrent_log_info(logger::cerr::log, "woke up!");
        std::unique_lock<std::mutex> _lock(*m_mutex);
        m_cond->notify_all();
        return concurrent::status::stop;
      }
      if ((p_reply.i % 2) == 0) {
        dispatcher::publish(request(5 * static_cast<uint32_t>(time(nullptr))));
      } else {
        dispatcher::publish(request(3 * static_cast<uint32_t>(time(nullptr))));
      }
      return concurrent::status::dont_stop;
    }

  private:
    int16_t m_counter = {0};
    std::condition_variable *m_cond;
    std::mutex *m_mutex;
  };

  struct replier {
    concurrent::status operator()(request &&p_request) {
      using namespace tenacitas;
      using namespace tenacitas;
      typedef concurrent::dispatcher_t<reply, logger::cerr::log> dispatcher;
      concurrent_log_info(logger::cerr::log, "request ", p_request);
      if (p_request.i == 0) {
        concurrent_log_info(logger::cerr::log, "stopping");
        return concurrent::status::stop;
      }
      if ((p_request.i % 2) == 0) {
        dispatcher::publish(reply(4 * static_cast<uint32_t>(time(nullptr))));
      } else {
        dispatcher::publish(reply(4 * static_cast<uint32_t>(time(nullptr))));
      }
      return concurrent::status::dont_stop;
    }
  };

  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<reply, logger::cerr::log> dispatcher_reply;
    typedef concurrent::dispatcher_t<request, logger::cerr::log>
        dispatcher_request;
    std::chrono::milliseconds _work_timeout(15000);
    dispatcher_reply::subscribe("req", requester(&m_cond, &m_mutex),
                                _work_timeout);
    dispatcher_request::subscribe("rep", replier(), _work_timeout);

    start();

    concurrent_log_info(logger::cerr::log, "------> waiting...");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond.wait(_lock);
    concurrent_log_info(logger::cerr::log, "------> done!");

    return true;
  }

  static const std::string desc() {
    return "\nTesting asynchronous work collaboration, and ending with "
           "message.";
  }

  static const std::string name() { return "dispatcher_002"; }

private:
  bool start() {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<request, logger::cerr::log> dispatcher;
    dispatcher::publish(request(static_cast<uint32_t>(time(nullptr))));
    return true;
  }

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

// ############################## main
struct dispatcher_003 {

  // ############################## loops
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sync_loop;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> rx_loop;

  // ############################## messages

  struct end {};

  struct msg_tpdo {
    msg_tpdo() { ++counter; }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const msg_tpdo & /*p_msg*/) {
      p_out << static_cast<uint64_t>(
          std::chrono::duration_cast<std::chrono::microseconds>(
              std::chrono::high_resolution_clock::now().time_since_epoch())
              .count());
      return p_out;
    }
    static uint16_t counter;
  };

  struct sync {
    concurrent::status operator()() {
      concurrent_log_debug(logger::cerr::log, "sync");
      return concurrent::status::dont_stop;
    }
  };

  // ############################## publishers
  struct rx {
    concurrent::status operator()() {
      concurrent_log_debug(logger::cerr::log, "rx");
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());
      concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::publish(
          msg_tpdo());

      return concurrent::status::dont_stop;
    }
  };

  // ############################## handler
  struct tx {
    tx() : m_id(++m_counter) {}

    concurrent::status operator()(msg_tpdo &&p_msg) {
      //    if (p_msg.counter > 20) {
      //      concurrent_log_debug(
      //        logger::cerr::log, "stoping because counter = ", p_msg.counter);
      //      return concurrent::status::stop;
      //    }
      concurrent_log_debug(logger::cerr::log, m_id, "|", p_msg);

      return concurrent::status::dont_stop;
    }

  private:
    uint16_t m_id = 0;
    static uint16_t m_counter;
  };

  dispatcher_003() {}

  bool operator()() {
    //    sync_loop _sync(
    //      std::chrono::milliseconds(100), sync(),
    //      std::chrono::milliseconds(2));

    rx_loop _rx(std::chrono::milliseconds(40), rx(),
                std::chrono::milliseconds(15));

    concurrent::dispatcher_t<msg_tpdo, logger::cerr::log>::subscribe(
        "tx", tx(), std::chrono::milliseconds(10));

    //    _sync.run();
    _rx.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(122));

    return true;
  }

  static const std::string desc() {
    return "\nTest inspired in a case brought by Fabio Costa";
  }

  static const std::string name() { return "dispatcher_003"; }
};

uint16_t dispatcher_003::msg_tpdo::counter = 0;
uint16_t dispatcher_003::tx::m_counter = 0;

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();

  tester::test _tester(argc, argv);

  run_test(_tester, dispatcher_000);
  run_test(_tester, dispatcher_001);
  run_test(_tester, dispatcher_002);
  run_test(_tester, dispatcher_003);
}
