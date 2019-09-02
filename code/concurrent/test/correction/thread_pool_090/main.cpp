#include <concurrent/business/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::business::thread_pool_t<msg_t> thread_pool_t;
typedef tenacitas::concurrent::business::sleeping_loop_t<void> sleeping_loop_t;

struct work
{
    bool operator()(msg_t&& p_msg)
    {
        m_msg = p_msg;
        cerr_test("handling msg ", m_msg);
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
                                  cerr_test("adding msg ", _msg);
                                  _pool.handle(std::move(_msg));
                                  return true;
                              },
                              std::chrono::milliseconds(300));

        _pool.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(100));

        _pool.run();
        _loop.run();

        cerr_test("sleeping for 10 secs");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        cerr_test("waking up after 10 secs");

        cerr_test("stopping the pool");
        _pool.stop();

        cerr_test("sleeping for 5 secs");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        cerr_test("waking up after 5 secs");

        cerr_test("runnig the pool");
        _pool.run();

        cerr_test("sleeping for 4 secs");
        std::this_thread::sleep_for(std::chrono::seconds(4));
        cerr_test("waking up after 4 secs");

        cerr_test(
          "consumed = ", _work.m_msg.counter(), ", provided = ", _value);
        if (_work.m_msg.counter() != _value) {
            cerr_error("Data value consumed should be equal to provided");
            return false;
        }

        return true;
    }
};
int
main(int argc, char** argv)
{
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
