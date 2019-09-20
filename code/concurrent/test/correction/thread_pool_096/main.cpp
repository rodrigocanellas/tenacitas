#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/test/msg_a.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::business::
  thread_pool_t<msg_t, tenacitas::logger::business::log>
    thread_pool_t;

using namespace tenacitas::logger::business;

struct work
{
    bool operator()(msg_t&& p_msg)
    {

        concurrent_log_test(log, "handling msg ", p_msg);
        if ((p_msg.counter() % 2) == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            ++m_timeout;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        return true;
    }
    uint16_t m_timeout = 0;
};

struct thread_pool_096
{
    bool operator()()
    {
        work _work;

        thread_pool_t _pool;
        _pool.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(300));

        for (uint16_t _i = 0; _i < 20; ++_i) {
            msg_t _msg(_i);
            concurrent_log_test(log, "adding msg ", _msg);
            _pool.handle(_msg);
        }

        concurrent_log_test(log, "start pool");
        _pool.run();

        concurrent_log_test(log, "sleeping for 1 s");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        concurrent_log_test(log, "waking up");

        concurrent_log_test(log, "timeout = ", _work.m_timeout);
        if (_work.m_timeout != 10) {
            concurrent_log_error(log, "# of timeout should be equal to 10");
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(thread_pool_096,
             argc,
             argv,
             "\n20 messages sent to the pool, and work function taking too "
             "long on messages which counter is even, and not too long for "
             "messages which counter is odd");
}
