#ifndef TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H
#define TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <vector>

#include <business/calendar/epoch.h>
#include <business/concurrent/sleeping_loop.h>
#include <test/concurrent/msg_a.h>
#include <business/concurrent/thread.h>
#include <business/concurrent/thread_pool.h>
#include <business/logger/cerr.h>
#include <business/logger/cout.h>
#include <business/concurrent/internal/log.h>

using namespace tenacitas;
//using namespace tenacitas::test;

template<uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester
{
    thread_pool_tester()
      : m_pool()
    {

        business::logger::configure_cout_log();
        business::logger::log::set_test();

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
    typedef test::concurrent::msg_a msg;
    typedef business::concurrent::thread_pool_t<msg, business::logger::log> thread_pool;
    typedef business::concurrent::thread thread;

    struct work
    {
        tenacitas::business::concurrent::result operator()(msg&& p_msg)
        {
            business::logger::concurrent_log_test(log, "consuming ", p_msg);
            std::this_thread::sleep_for(
              std::chrono::milliseconds(work_sleep_ms));

            return tenacitas::business::concurrent::result::dont_stop;
        }
    };

    void produce()
    {
        for (uint32_t _count_msg = 0; _count_msg < num_msgs; ++_count_msg) {
            msg _msg(_count_msg);
            business::logger::concurrent_log_test(log, "adding ", _msg);
            m_pool.handle(_msg);
        }
    }

  private:
    thread_pool m_pool;
};

#endif
