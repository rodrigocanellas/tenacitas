#include <concurrent/business/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::business::thread_pool_t<msg_t> thread_pool_t;

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

struct thread_pool_095
{

    bool operator()()
    {
        work _work;

        thread_pool_t _pool;

        _pool.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(200));

        msg_t _msg(18);
        cerr_test("adding msg ", _msg);
        _pool.handle(_msg);

        cerr_test("start pool");
        _pool.run();

        cerr_test("sleeping for 1 s");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cerr_test("waking up");

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(thread_pool_095, argc, argv, "\nWork function taking too long");
}
