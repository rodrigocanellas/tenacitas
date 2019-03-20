#ifndef TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H
#define TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <vector>

#include <calendar/bus/epoch.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/test/msg_a.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/thread_pool.h>
#include <logger/bus/cerr.h>
#include <logger/bus/cout.h>

template<uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester
{
    thread_pool_tester()
      : m_pool()
    {
        tenacitas::logger::bus::configure_cout_log();
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
    typedef tenacitas::concurrent::bus::thread_pool<msg_t> thread_pool_t;
    typedef tenacitas::concurrent::bus::thread thread_t;

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
        //        uint32_t _producer = 0;
        //        for (producers_t::iterator _ite = m_producers.begin();
        //             _ite != m_producers.end();
        //             ++_ite) {
        //            ++_producer;
        //            *_ite = thread_t([this, _producer]() {
        //                for (uint32_t _count_msg = 0; _count_msg < num_msgs;
        //                     ++_count_msg) {
        //                    msg_t _msg(_producer, _count_msg);
        //                    cerr_test("adding ", _msg);
        //                    this->m_pool.handle(_msg);
        //                }
        //            });
        //        }
    }

  private:
    thread_pool_t m_pool;
};

#endif
