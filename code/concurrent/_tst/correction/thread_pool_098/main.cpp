/// \example
#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/thread_pool.h>
#include <concurrent/_bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

#include <chrono>

using namespace tenacitas;

typedef concurrent::_bus::thread_pool_t<int32_t, logger::cerr::log> thread_pool;
typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

using namespace tenacitas;
using namespace tenacitas;

template <typename t_notifier> struct work_1 {
  work_1(t_notifier *p_notifier) : m_notifier(p_notifier) {}

  concurrent::_bus::work_status operator()(int32_t &&p_value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(170));
    concurrent_log_debug(logger::cerr::log, "work 1 handling msg ", p_value);
    if (p_value > 200) {
      m_notifier->stop("work1");
      return concurrent::_bus::work_status::stop;
    }
    return concurrent::_bus::work_status::dont_stop;
  }

private:
  t_notifier *m_notifier = nullptr;
};

struct thread_pool_098 {
  thread_pool_098() {
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));

    m_pool.add_work(4, [this]() { return work_1<thread_pool_098>(this); },
                    std::chrono::milliseconds(200));
  }

  bool operator()() {
    //    m_pool.add_work(
    //      4, [this]() { return work_1(this); },
    //      std::chrono::milliseconds(200));

    int32_t _value = 0;

    m_pool.run();

    m_pool.handle(_value);
    while (true) {
      std::unique_lock<std::mutex> _lock(m_mutex_stop);
      if (m_cond_stop.wait_for(_lock, std::chrono::milliseconds(50),
                               [this]() -> bool { return m_stop; })) {
        concurrent_log_debug(logger::cerr::log, "notified");
        m_pool.stop();
        break;
      }
      //      if (m_stop) {
      //        m_pool.stop();
      //        break;
      //      }
      m_pool.handle(++_value);
    }
    concurrent_log_debug(logger::cerr::log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    concurrent_log_debug(logger::cerr::log, "woke up");
    return true;
  }

  void stop(const std::string &p_str) {
    concurrent_log_debug(logger::cerr::log, "stop called with ", p_str);
    m_stop = true;
    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_cond_stop.notify_all();
  }

  static std::string desc() { return "\nStopping when a max value is hit"; }

  static std::string name() { return "thread_pool_098"; }

private:
  bool m_stop = false;
  thread_pool m_pool;
  std::condition_variable m_cond_stop;
  std::mutex m_mutex_stop;
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_098>(argc, argv);
}
