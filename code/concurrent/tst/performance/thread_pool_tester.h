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
#include <concurrent/business/internal/log.h>

using namespace tenacitas;
//using namespace tenacitas::test;

template<uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester
{
    thread_pool_tester()
      : m_pool()
    {

        logger::business::configure_cout_log();
        logger::business::log::set_test();

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
    typedef concurrent::test::msg_a msg;
    typedef concurrent::business::thread_pool_t<msg, logger::business::log> thread_pool;
    typedef concurrent::business::thread thread;

    struct work
    {
        tenacitas::concurrent::business::work_status operator()(msg&& p_msg)
        {
            logger::business::concurrent_log_test(log, "consuming ", p_msg);
            std::this_thread::sleep_for(
              std::chrono::milliseconds(work_sleep_ms));

            return tenacitas::concurrent::business::work_status::dont_stop;
        }
    };

    void produce()
    {
        for (uint32_t _count_msg = 0; _count_msg < num_msgs; ++_count_msg) {
            msg _msg(_count_msg);
            logger::business::concurrent_log_test(log, "adding ", _msg);
            m_pool.handle(_msg);
        }
    }

  private:
    thread_pool m_pool;
};

#endif
