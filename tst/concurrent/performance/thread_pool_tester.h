#ifndef TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H
#define TENACITAS_CONCURRENT_TST_THREAD_POOL_TEST_HELPER_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <vector>

#include <calendar/epoch.h>
#include <concurrent/internal/log.h>
#include <concurrent/msg_a.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/thread.h>
#include <concurrent/thread_pool.h>
#include <logger/cerr/log.h>

using namespace tenacitas;

template <uint32_t num_consumers, uint32_t num_msgs, uint32_t work_sleep_ms>
struct thread_pool_tester {
  thread_pool_tester() : m_pool() {

    logger::cerr::log::set_debug();

    m_pool.add_work(
        num_consumers, [] { return work(); }, std::chrono::milliseconds(15000));
  }

  void operator()() {
    m_pool.start();
    produce();
  }

private:
  typedef concurrent::msg_a<int32_t> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;
  typedef concurrent::thread thread;

  struct work {
    status::result operator()(msg &&p_msg) {
      concurrent_debug(logger::cerr::log, "consuming ", p_msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(work_sleep_ms));

      return status::ok;
    }
  };

  void produce() {
    for (uint32_t _count_msg = 0; _count_msg < num_msgs; ++_count_msg) {
      msg _msg(_count_msg);
      concurrent_debug(logger::cerr::log, "adding ", _msg);
      m_pool.handle(_msg);
    }
  }

private:
  thread_pool m_pool;
};

#endif
