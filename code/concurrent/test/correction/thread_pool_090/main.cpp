#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <concurrent/test/msg_a.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

typedef tenacitas::logger::business::log log_t;
typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::business::thread_pool_t<msg_t, log_t>
  thread_pool_t;
typedef tenacitas::concurrent::business::sleeping_loop_t<void, log_t>
  sleeping_loop_t;

struct work
{
    bool operator()(msg_t&& p_msg)
    {
        m_msg = p_msg;
        concurrent_log_test(log_t, "handling msg ", m_msg);
        return true;
    }
    msg_t m_msg;
};

struct thread_pool_090
{
    bool operator()()
    {

        work _work;

        thread_pool_t _pool;
        uint32_t _value = 0;

        sleeping_loop_t _loop(std::chrono::milliseconds(500),
                              [&_pool, &_value]() {
                                  msg_t _msg(++_value);
                                  concurrent_log_test(
                                    log_t, "adding msg ", _msg);
                                  _pool.handle(std::move(_msg));
                                  return true;
                              },
                              std::chrono::milliseconds(300));

        _pool.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(100));

        _pool.run();
        _loop.run();

        concurrent_log_test(log_t, "sleeping for 10 secs");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        concurrent_log_test(log_t, "waking up after 10 secs");

        concurrent_log_test(log_t, "stopping the pool");
        _pool.stop();

        concurrent_log_test(log_t, "sleeping for 5 secs");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        concurrent_log_test(log_t, "waking up after 5 secs");

        concurrent_log_test(log_t, "runnig the pool");
        _pool.run();

        concurrent_log_test(log_t, "sleeping for 4 secs");
        std::this_thread::sleep_for(std::chrono::seconds(4));
        concurrent_log_test(log_t, "waking up after 4 secs");

        concurrent_log_test(
          log_t, "consumed = ", _work.m_msg.counter(), ", provided = ", _value);
        if (_work.m_msg.counter() != _value) {
            concurrent_log_error(
              log_t, "Data value consumed should be equal to provided");
            return false;
        }

        return true;
    }
};
int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(thread_pool_090,
             argc,
             argv,
             "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
             "'thread_pool' with one consumer."

             "\nThe main thread will sleep for 10 secs, the 'thread_pool' will "
             "stop; main thread will sleep for 5 secs; 'thread_pool' will run "
             "again; main thread will sleep for 4 secs."

             "\nThe messages added while the pool was stopped are handled when "
             "the pool runs again."

             "\nThe amount of data consumed must be equal to the provided");
}
