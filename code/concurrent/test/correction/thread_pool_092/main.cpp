#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/business/traits.h>
#include <concurrent/test/msg_a.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

using namespace tenacitas::concurrent::business;
using namespace tenacitas::logger::business;

typedef tenacitas::concurrent::tst::msg_a msg;
typedef tenacitas::concurrent::business::
thread_pool_t<msg, tenacitas::logger::business::log>
thread_pool;

using namespace tenacitas::logger::business;

struct work
{
  result operator()(msg&& p_msg)
  {
    m_msg = p_msg;
    concurrent_log_test(log, "handling msg ", m_msg);
    return result::dont_stop;
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
      concurrent_log_test(log, "adding msg ", _msg);
      _pool_1.handle(_msg);
    }

    thread_pool _pool_2(std::move(_pool_1));

    _pool_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    concurrent_log_test(log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 19) {
      concurrent_log_error(log,
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
  tenacitas::logger::business::configure_cerr_log();
  run_test(thread_pool_092,
           argc,
           argv,
           "\n20 messages added to a 'thread_pool' with one consumer; "
           "'thread_pool' moved; new 'thread_pool' started");
}
