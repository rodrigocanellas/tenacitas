#ifndef TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H
#define TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <vector>

#include <calendar/business/epoch.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/business/thread.h>
#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>
#include <logger/business/cout.h>

template<uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester
{
    thread_pool_tester()
      : m_pool()
    {
        tenacitas::logger::business::configure_cout_log();
        cout_set_test();

        m_pool.add_work(num_consumers,
                        [] { return work(); },
                        std::chrono::milliseconds(15000));
    }

    void operator()()
    {
        m_pool.run();
        produce();
    }

  private:
    typedef tenacitas::concurrent::tst::msg_a msg_t;
    typedef tenacitas::concurrent::business::thread_pool_t<msg_t> thread_pool_t;
    typedef tenacitas::concurrent::business::thread thread_t;

    struct work
    {
        bool operator()(msg_t&& p_msg)
        {
            cerr_test("consuming ", p_msg);
            std::this_thread::sleep_for(
              std::chrono::milliseconds(work_sleep_ms));

            return true;
        }
    };

    void produce()
    {

        for (uint32_t _count_msg = 0; _count_msg < num_msgs; ++_count_msg) {
            msg_t _msg(_count_msg);
            cerr_test("adding ", _msg);
            m_pool.handle(_msg);
        }
    }

  private:
    thread_pool_t m_pool;
};

#endif
