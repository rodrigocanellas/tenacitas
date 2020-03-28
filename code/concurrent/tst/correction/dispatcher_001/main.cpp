/// \example
#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>

#include <concurrent/bus/dispatcher.h>
#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>


using namespace tenacitas;
using namespace tenacitas;

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
  concurrent::bus::work_status start()
  {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::bus::dispatcher_t<request, logger::cerr::log> dispatcher;
    dispatcher::publish(request(time(nullptr)));
    return concurrent::bus::work_status::dont_stop;
  }

  concurrent::bus::work_status operator()(reply&& p_reply)
  {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::bus::dispatcher_t<request, logger::cerr::log> dispatcher;
    concurrent_log_debug( "reply ", p_reply);
    if (m_counter++ > 100) {
      concurrent_log_debug_info(logger::cerr::log, "counter = ", m_counter, ", stopping");
      dispatcher::publish(request(0));
      return concurrent::bus::work_status::stop;
    }
    if ((p_reply.i % 2) == 0) {
      dispatcher::publish(request(5 * time(nullptr)));
    } else {
      dispatcher::publish(request(3 * time(nullptr)));
    }
    return concurrent::bus::work_status::dont_stop;
  }

private:
  int16_t m_counter = { 0 };
};

struct replier
{
  concurrent::bus::work_status operator()(request&& p_request)
  {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::bus::dispatcher_t<reply, logger::cerr::log> dispatcher;
    concurrent_log_debug( "request ", p_request);
    if (p_request.i == 0) {
      concurrent_log_debug_info(logger::cerr::log, "stopping");
      return concurrent::bus::work_status::stop;
    }
    if ((p_request.i % 2) == 0) {
      dispatcher::publish(reply(4 * time(nullptr)));
    } else {
      dispatcher::publish(reply(4 * time(nullptr)));
    }
    return concurrent::bus::work_status::dont_stop;
  }
};

// ############################## subscribers
struct dispatcher_001
{

  bool operator()()
  {
    using namespace tenacitas;
    using namespace tenacitas;
    typedef concurrent::bus::dispatcher_t<reply, logger::cerr::log> dispatcher_reply;
    typedef concurrent::bus::dispatcher_t<request, logger::cerr::log> dispatcher_request;
    std::chrono::milliseconds _work_timeout(500);
    dispatcher_reply::subscribe("req", requester(), _work_timeout);
    dispatcher_request::subscribe("rep", replier(), _work_timeout);

    requester().start();

    concurrent_log_debug( "------> starting to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_debug( "------> waking up");

    return true;
  }

private:
};

int
main(int argc, char** argv)
{
  logger::bus::configure_cerr_log();
  run_test(
    dispatcher_001, argc, argv, "\nTesting asynchronous work collaboration.")
}
