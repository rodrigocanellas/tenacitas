/// \example
#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>

#include <concurrent/business/traits.h>
#include <concurrent/business/dispatcher.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::concurrent::business;
using namespace tenacitas::logger::business;

// ############################## messages
struct request
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                        const request& p_msg)
  {
    p_out << "|msg a|i|" << p_msg.i;
    return p_out;
  }

  explicit request(int16_t p_i = 0)
    : i(p_i)
  {}

  int32_t i = { 0 };
};

struct reply
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                        const reply& p_msg)
  {
    p_out << "|msg a|i|" << p_msg.i;
    return p_out;
  }

  explicit reply(int16_t p_i = 0)
    : i(p_i)
  {}

  int32_t i = { 0 };
};

// ############################## processors
struct requester
{
  result start()
  {
    using namespace tenacitas::logger::business;
    using namespace tenacitas::concurrent::business;
    typedef dispatcher_t<request, log> dispatcher;
    dispatcher::publish(request(time(nullptr)));
    return result::dont_stop;
  }

  result operator()(reply&& p_reply)
  {
    using namespace tenacitas::logger::business;
    using namespace tenacitas::concurrent::business;
    typedef dispatcher_t<request, log> dispatcher;
    concurrent_log_test(log, "reply ", p_reply);
    if (m_counter++ > 100) {
      concurrent_log_info(log, "counter = ", m_counter, ", stopping");
      dispatcher::publish(request(0));
      return result::stop;
    }
    if ((p_reply.i % 2) == 0) {
      dispatcher::publish(request(5 * time(nullptr)));
    } else {
      dispatcher::publish(request(3 * time(nullptr)));
    }
    return result::dont_stop;
  }

private:
  int16_t m_counter = { 0 };
};

struct replier
{
  result operator()(request&& p_request)
  {
    using namespace tenacitas::logger::business;
    using namespace tenacitas::concurrent::business;
    typedef dispatcher_t<reply, log> dispatcher;
    concurrent_log_test(log, "request ", p_request);
    if (p_request.i == 0) {
      concurrent_log_info(log, "stopping");
      return result::stop;
    }
    if ((p_request.i % 2) == 0) {
      dispatcher::publish(reply(4 * time(nullptr)));
    } else {
      dispatcher::publish(reply(4 * time(nullptr)));
    }
    return result::dont_stop;
  }
};

// ############################## subscribers
struct dispatcher_001
{

  bool operator()()
  {
    using namespace tenacitas::logger::business;
    using namespace tenacitas::concurrent::business;
    typedef dispatcher_t<reply, log> dispatcher_reply;
    typedef dispatcher_t<request, log> dispatcher_request;
    std::chrono::milliseconds _work_timeout(500);
    dispatcher_reply::subscribe("req", requester(), _work_timeout);
    dispatcher_request::subscribe("rep", replier(), _work_timeout);

    requester().start();

    concurrent_log_test(log, "------> starting to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_test(log, "------> waking up");

    return true;
  }

private:
};

int
main(int argc, char** argv)
{
  tenacitas::logger::business::configure_cerr_log();
  run_test(
        dispatcher_001, argc, argv, "\nTesting asynchronous work collaboration.")
}
