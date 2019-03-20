#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/bus/thread_pool.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

#include <chrono>

typedef tenacitas::concurrent::tst::msg_a msg_t;
typedef tenacitas::concurrent::bus::thread_pool<msg_t> thread_pool_t;

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

struct thread_pool_092
{
    bool operator()()
    {
        work _work;

        thread_pool_t _pool_1;
        _pool_1.add_work(
          [&_work](msg_t&& p_msg) { return _work(std::move(p_msg)); },
          std::chrono::milliseconds(500));

        for (uint16_t _i = 0; _i < 20; ++_i) {
            msg_t _msg(_i);
            cerr_test("adding msg ", _msg);
            _pool_1.handle(_msg);
        }

        thread_pool_t _pool_2(std::move(_pool_1));

        _pool_2.run();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        cerr_test("consumed = ", _work.m_msg.counter());
        if (_work.m_msg.counter() != 19) {
            cerr_error("Data value consumed should be equal to 19");
            return false;
        }

        return true;
    }

  private:
};

int
main(int argc, char** argv)
{
    run_test(thread_pool_092,
             argc,
             argv,
             "\n20 messages added to a 'thread_pool' with one consumer; "
             "'thread_pool' moved; new 'thread_pool' started");
}
