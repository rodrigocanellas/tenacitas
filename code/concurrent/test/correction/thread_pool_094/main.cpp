#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/business/traits.h>
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
typedef concurrent::business::
sleeping_loop_t<void, logger::business::log>
sleeping_loop;

using namespace tenacitas;

struct work
{
  concurrent::business::result operator()(msg&& p_msg)
  {
    m_msg = p_msg;
    concurrent_log_test(logger::business::log, "handling msg ", m_msg);
    return concurrent::business::result::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_094
{

  bool operator()()
  {
    work _work;
    thread_pool _pool;

    _pool.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(1000));

    for (uint16_t _i = 0; _i < 200; ++_i) {
      msg _msg(_i);
      concurrent_log_test(logger::business::log, "adding msg ", _msg);
      _pool.handle(_msg);
    }

    concurrent_log_test(logger::business::log, "first start");
    _pool.run();

    concurrent_log_test(logger::business::log, "sleeping for 2 s");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    concurrent_log_test(logger::business::log, "waking up");

    concurrent_log_test(logger::business::log, "second start");
    _pool.run();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_test(logger::business::log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 199) {
      concurrent_log_error(logger::business::log,
                           "Data value consumed should be equal to 199");
      return false;
    }

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::business::configure_cerr_log();

  run_test(thread_pool_094, argc, argv, "\nStarting a 'thread_pool' twice");
}
