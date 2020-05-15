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
using namespace tenacitas;

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
  concurrent::work_status start() {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<request, logger::cerr::log>
        dispatcher;
    dispatcher::publish(request(time(nullptr)));
    return concurrent::work_status::dont_stop;
  }

  concurrent::work_status operator()(reply &&p_reply) {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<request, logger::cerr::log>
        dispatcher;
    concurrent_log_debug(logger::cerr::log, "reply ", p_reply);
    if (m_counter++ > 100) {
      concurrent_log_info(logger::cerr::log, "counter = ", m_counter,
                          ", stopping");
      dispatcher::publish(request(0));
      return concurrent::work_status::stop;
    }
    if ((p_reply.i % 2) == 0) {
      dispatcher::publish(request(5 * time(nullptr)));
    } else {
      dispatcher::publish(request(3 * time(nullptr)));
    }
    return concurrent::work_status::dont_stop;
  }

private:
  int16_t m_counter = {0};
};

struct replier {
  concurrent::work_status operator()(request &&p_request) {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<reply, logger::cerr::log> dispatcher;
    concurrent_log_debug(logger::cerr::log, "request ", p_request);
    if (p_request.i == 0) {
      concurrent_log_info(logger::cerr::log, "stopping");
      return concurrent::work_status::stop;
    }
    if ((p_request.i % 2) == 0) {
      dispatcher::publish(reply(4 * time(nullptr)));
    } else {
      dispatcher::publish(reply(4 * time(nullptr)));
    }
    return concurrent::work_status::dont_stop;
  }
};

// ############################## subscribers
struct dispatcher_001 {

  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::dispatcher_t<reply, logger::cerr::log>
        dispatcher_reply;
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

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<dispatcher_001>(argc, argv);
}
