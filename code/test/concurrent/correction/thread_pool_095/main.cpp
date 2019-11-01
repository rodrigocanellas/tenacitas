#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/concurrent/thread_pool.h>
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
    m_msg = p_msg;
    concurrent_log_test(logger::log, "handling msg ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_095
{

  bool operator()()
  {
    work _work;

    thread_pool _pool;

    _pool.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(200));

    msg _msg(18);
    concurrent_log_test(logger::log, "adding msg ", _msg);
    _pool.handle(_msg);

    concurrent_log_test(logger::log, "start pool");
    _pool.run();

    concurrent_log_test(logger::log, "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    concurrent_log_test(logger::log, "waking up");

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(thread_pool_095, argc, argv, "\nWork function taking too long");
}
