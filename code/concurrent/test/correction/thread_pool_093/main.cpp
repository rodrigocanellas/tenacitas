#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/test/msg_a.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

using namespace tenacitas::logger::business;

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::business::
  thread_pool_t<msg_t, tenacitas::logger::business::log>
    thread_pool_t;
typedef tenacitas::concurrent::business::
  sleeping_loop_t<void, tenacitas::logger::business::log>
    sleeping_loop_t;

struct work
{
    bool operator()(msg_t&& p_msg)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        m_msg = p_msg;
        concurrent_log_test(log, "handling msg ", m_msg);
        return true;
    }
    msg_t m_msg;
};

struct thread_pool_093
{
    bool operator()()
    {
        work _work;
        thread_pool_t _pool_1;
        _pool_1.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(501));

        for (uint16_t _i = 0; _i < 200; ++_i) {
            msg_t _msg(_i);
            concurrent_log_test(log, "adding msg ", _msg);
            _pool_1.handle(_msg);
        }

        concurrent_log_test(log, "starting pool");
        _pool_1.run();

        concurrent_log_test(log, "sleeping for 2 s");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        concurrent_log_test(log, "waking up");

        concurrent_log_test(log, "stopping pool");
        _pool_1.stop();

        concurrent_log_test(log, "moving pool");
        thread_pool_t _pool_2(std::move(_pool_1));

        concurrent_log_test(log, "sleeping for 3 s");
        std::this_thread::sleep_for(std::chrono::seconds(3));

        concurrent_log_test(log, "starting new pool");
        _pool_2.run();

        std::this_thread::sleep_for(std::chrono::seconds(10));

        concurrent_log_test(log, "consumed = ", _work.m_msg.counter());
        if (_work.m_msg.counter() != 199) {
            concurrent_log_error(log,
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
    tenacitas::logger::business::configure_cerr_log();
    run_test(
      thread_pool_093,
      argc,
      argv,
      "\n200 messages added to a 'thread_pool' with one consumer, each "
      "sleeping for 50 ms; 'thread_pool' started; main thread sleeps for 2 s; "
      "'thread_pool' stopped; 'thread_pool' moved; main thread sleeps for 2 s; "
      "new 'thread_pool' started");
}
