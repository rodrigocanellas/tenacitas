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
    m_msg = p_msg;
    concurrent_log_test(logger::log, "handling msg ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_092
{
  bool operator()()
  {
    work _work;

    thread_pool _pool_1;
    _pool_1.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(500));

    for (uint16_t _i = 0; _i < 20; ++_i) {
      msg _msg(_i);
      concurrent_log_test(logger::log, "adding msg ", _msg);
      _pool_1.handle(_msg);
    }

    thread_pool _pool_2(std::move(_pool_1));

    _pool_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    concurrent_log_test(logger::log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 19) {
      concurrent_log_error(logger::log,
                           "Data value consumed should be equal to 19");
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
  run_test(thread_pool_092,
           argc,
           argv,
           "\n20 messages added to a 'thread_pool' with one consumer; "
           "'thread_pool' moved; new 'thread_pool' started");
}
