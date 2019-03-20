#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/bus/thread_pool.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

#include <chrono>

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::bus::thread_pool<msg_t> thread_pool_t;
typedef tenacitas::concurrent::bus::sleeping_loop<void> sleeping_loop_t;

struct work
{
    bool operator()(msg_t&& p_msg)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        m_msg = p_msg;
        cerr_test("handling msg ", m_msg);
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
            cerr_test("adding msg ", _msg);
            _pool_1.handle(_msg);
        }

        cerr_test("starting pool");
        _pool_1.run();

        cerr_test("sleeping for 2 s");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cerr_test("waking up");

        cerr_test("stopping pool");
        _pool_1.stop();

        cerr_test("moving pool");
        thread_pool_t _pool_2(std::move(_pool_1));

        cerr_test("sleeping for 3 s");
        std::this_thread::sleep_for(std::chrono::seconds(3));

        cerr_test("starting new pool");
        _pool_2.run();

        std::this_thread::sleep_for(std::chrono::seconds(10));

        cerr_test("consumed = ", _work.m_msg.counter());
        if (_work.m_msg.counter() != 199) {
            cerr_error("Data value consumed should be equal to 199");
            return false;
        }

        return true;
    }

  private:
};

int
main(int argc, char** argv)
{
    run_test(
      thread_pool_093,
      argc,
      argv,
      "\n200 messages added to a 'thread_pool' with one consumer, each "
      "sleeping for 50 ms; 'thread_pool' started; main thread sleeps for 2 s; "
      "'thread_pool' stopped; 'thread_pool' moved; main thread sleeps for 2 s; "
      "new 'thread_pool' started");
}
