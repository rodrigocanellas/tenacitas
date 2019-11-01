#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/thread_pool.h>
#include <business/concurrent/traits.h>
#include <test/concurrent/msg_a.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

#include <chrono>

using namespace tenacitas::business;
using namespace tenacitas::business;

typedef tenacitas::test::concurrent::msg_a msg;
typedef concurrent::
thread_pool_t<msg, logger::log>
thread_pool;

using namespace tenacitas::business;

struct work
{
  concurrent::result operator()(msg&& p_msg)
  {

    concurrent_log_test(logger::log, "handling msg ", p_msg);
    if ((p_msg.counter() % 2) == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      ++m_timeout;
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return concurrent::result::dont_stop;
  }
  uint16_t m_timeout = 0;
};

struct thread_pool_096
{
  bool operator()()
  {
    work _work;

    thread_pool _pool;
    _pool.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(300));

    for (uint16_t _i = 0; _i < 20; ++_i) {
      msg _msg(_i);
      concurrent_log_test(logger::log, "adding msg ", _msg);
      _pool.handle(_msg);
    }

    concurrent_log_test(logger::log, "start pool");
    _pool.run();

    concurrent_log_test(logger::log, "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_test(logger::log, "waking up");

    concurrent_log_test(logger::log, "timeout = ", _work.m_timeout);
    if (_work.m_timeout != 10) {
      concurrent_log_error(logger::log, "# of timeout should be equal to 10");
      return false;
    }

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(thread_pool_096,
           argc,
           argv,
           "\n20 messages sent to the pool, and work function taking too "
           "long on messages which counter is even, and not too long for "
           "messages which counter is odd");
}
