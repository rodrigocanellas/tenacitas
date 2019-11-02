#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/traits.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/test/msg_a.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

using namespace tenacitas;
using namespace tenacitas;

typedef tenacitas::concurrent::test::msg_a msg;
typedef concurrent::business::
thread_pool_t<msg, logger::business::log>
thread_pool;

using namespace tenacitas;

struct work
{
  concurrent::business::work_status operator()(msg&& p_msg)
  {
    m_msg = p_msg;
    concurrent_log_test(logger::business::log, "handling msg ", m_msg);
    return concurrent::business::work_status::dont_stop;
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
    concurrent_log_test(logger::business::log, "adding msg ", _msg);
    _pool.handle(_msg);

    concurrent_log_test(logger::business::log, "start pool");
    _pool.run();

    concurrent_log_test(logger::business::log, "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    concurrent_log_test(logger::business::log, "waking up");

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::business::configure_cerr_log();
  run_test(thread_pool_095, argc, argv, "\nWork function taking too long");
}
