/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <sstream>
#include <string>

#include <business/concurrent/dispatcher.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

// ############################## messages
struct request
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                        const request& p_msg)
  {
    p_out << "|msg a|i|" << p_msg.i;
    return p_out;
  }

  explicit request(uint32_t p_i = 0)
    : i(p_i)
  {}

  uint32_t i = { 0 };
};

struct reply
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                        const reply& p_msg)
  {
    p_out << "|msg a|i|" << p_msg.i;
    return p_out;
  }

  explicit reply(uint32_t p_i = 0)
    : i(p_i)
  {}

  uint32_t i = { 0 };
};

// ############################## processors
struct requester
{
  requester(std::condition_variable* p_cond, std::mutex* p_mutex)
    : m_cond(p_cond)
    , m_mutex(p_mutex)
  {}

  concurrent::result operator()(reply&& p_reply)
  {
    using namespace tenacitas::business;
    using namespace tenacitas::business;
    typedef concurrent::dispatcher_t<request, logger::log> dispatcher;
    concurrent_log_test(logger::log, "reply ", p_reply);
    if (m_counter++ > 100) {
      concurrent_log_info(logger::log, "counter = ", m_counter, ", stopping");
      dispatcher::publish(request(0));
      concurrent_log_test(logger::log, "sleeping...");
      std::this_thread::sleep_for(std::chrono::seconds(8));
      concurrent_log_test(logger::log, "woke up!");
      std::unique_lock<std::mutex> _lock(*m_mutex);
      m_cond->notify_all();
      return concurrent::result::stop;
    }
    if ((p_reply.i % 2) == 0) {
      dispatcher::publish(request(5 * static_cast<uint32_t>(time(nullptr))));
    } else {
      dispatcher::publish(request(3 * static_cast<uint32_t>(time(nullptr))));
    }
    return concurrent::result::dont_stop;
  }

private:
  int16_t m_counter = { 0 };
  std::condition_variable* m_cond;
  std::mutex* m_mutex;
};

struct replier
{
  concurrent::result operator()(request&& p_request)
  {
    using namespace tenacitas::business;
    using namespace tenacitas::business;
    typedef concurrent::dispatcher_t<reply, logger::log> dispatcher;
    concurrent_log_test(logger::log, "request ", p_request);
    if (p_request.i == 0) {
      concurrent_log_info(logger::log, "stopping");
      return concurrent::result::stop;
    }
    if ((p_request.i % 2) == 0) {
      dispatcher::publish(reply(4 * static_cast<uint32_t>(time(nullptr))));
    } else {
      dispatcher::publish(reply(4 * static_cast<uint32_t>(time(nullptr))));
    }
    return concurrent::result::dont_stop;
  }
};

struct dispatcher_002
{

  bool operator()()
  {
    using namespace tenacitas::business;
    using namespace tenacitas::business;
    typedef concurrent::dispatcher_t<reply, logger::log> dispatcher_reply;
    typedef concurrent::dispatcher_t<request, logger::log> dispatcher_request;
    std::chrono::milliseconds _work_timeout(15000);
    dispatcher_reply::subscribe(
      "req", requester(&m_cond, &m_mutex), _work_timeout);
    dispatcher_request::subscribe("rep", replier(), _work_timeout);

    start();

    concurrent_log_test(logger::log, "------> waiting...");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond.wait(_lock);
    concurrent_log_test(logger::log, "------> done!");

    return true;
  }

private:
  bool start()
  {
    using namespace tenacitas::business;
    using namespace tenacitas::business;
    typedef concurrent::dispatcher_t<request, logger::log> dispatcher;
    dispatcher::publish(request(static_cast<uint32_t>(time(nullptr))));
    return true;
  }

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
    dispatcher_002,
    argc,
    argv,
    "\nTesting asynchronous work collaboration, and ending with message.")
}
