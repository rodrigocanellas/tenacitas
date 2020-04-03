#ifndef TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H
#define TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <vector>

#include <calendar/_bus/epoch.h>
#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/thread.h>
#include <concurrent/_bus/thread_pool.h>
#include <concurrent/_tst/msg_a.h>
#include <logger/cerr/log.h>

using namespace tenacitas;

template <uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester {
  thread_pool_tester() : m_pool() {

    logger::cerr::log::set_debug();

    m_pool.add_work(num_consumers, [] { return work(); },
                    std::chrono::milliseconds(15000));
  }

  void operator()() {
    m_pool.run();
    produce();
  }

private:
  typedef concurrent::_tst::msg_a msg;
  typedef concurrent::_bus::thread_pool_t<msg, logger::cerr::log> thread_pool;
  typedef concurrent::_bus::thread thread;

  struct work {
    concurrent::_bus::work_status operator()(msg &&p_msg) {
      concurrent_log_debug(logger::cerr::log, "consuming ", p_msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(work_sleep_ms));

      return concurrent::_bus::work_status::dont_stop;
    }
  };

  void produce() {
    for (uint32_t _count_msg = 0; _count_msg < num_msgs; ++_count_msg) {
      msg _msg(_count_msg);
      concurrent_log_debug(logger::cerr::log, "adding ", _msg);
      m_pool.handle(_msg);
    }
  }

private:
  thread_pool m_pool;
};

#endif
