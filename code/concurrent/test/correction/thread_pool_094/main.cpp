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

struct thread_pool_094
{

    bool operator()()
    {
        work _work;
        thread_pool_t _pool;

        _pool.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(1000));

        for (uint16_t _i = 0; _i < 200; ++_i) {
            msg_t _msg(_i);
            cerr_test("adding msg ", _msg);
            _pool.handle(_msg);
        }

        cerr_test("first start");
        _pool.run();

        cerr_test("sleeping for 2 s");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cerr_test("waking up");

        cerr_test("second start");
        _pool.run();

        std::this_thread::sleep_for(std::chrono::seconds(3));

        cerr_test("consumed = ", _work.m_msg.counter());
        if (_work.m_msg.counter() != 199) {
            cerr_error("Data value consumed should be equal to 199");
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(thread_pool_094, argc, argv, "\nStarting a 'thread_pool' twice");
}
