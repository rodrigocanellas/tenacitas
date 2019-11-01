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
typedef concurrent::
sleeping_loop_t<void, logger::log>
sleeping_loop;

struct work
{
  concurrent::result operator()(msg&& p_msg)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m_msg = p_msg;
    concurrent_log_test(logger::log, "handling msg ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_093
{
  bool operator()()
  {
    work _work;
    thread_pool _pool_1;
    _pool_1.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(501));

    for (uint16_t _i = 0; _i < 200; ++_i) {
      msg _msg(_i);
      concurrent_log_test(logger::log, "adding msg ", _msg);
      _pool_1.handle(_msg);
    }

    concurrent_log_test(logger::log, "starting pool");
    _pool_1.run();

    concurrent_log_test(logger::log, "sleeping for 2 s");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    concurrent_log_test(logger::log, "waking up");

    concurrent_log_test(logger::log, "stopping pool");
    _pool_1.stop();

    concurrent_log_test(logger::log, "moving pool");
    thread_pool _pool_2(std::move(_pool_1));

    concurrent_log_test(logger::log, "sleeping for 3 s");
    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_test(logger::log, "starting new pool");
    _pool_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_test(logger::log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 199) {
      concurrent_log_error(logger::log,
                           "Data value consumed should be equal to 199");
      return false;
    }

    return true;
  }

private:
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
        thread_pool_093,
        argc,
        argv,
        "\n200 messages added to a 'thread_pool' with one consumer, each "
        "sleeping for 50 ms; 'thread_pool' started; main thread sleeps for 2 s; "
        "'thread_pool' stopped; 'thread_pool' moved; main thread sleeps for 2 s; "
        "new 'thread_pool' started");
}
