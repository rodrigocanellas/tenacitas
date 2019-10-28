/// \example
#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#include <chrono>

typedef tenacitas::concurrent::business::
  thread_pool_t<int32_t, tenacitas::logger::business::log>
    thread_pool_t;
typedef tenacitas::concurrent::business::
  sleeping_loop_t<void, tenacitas::logger::business::log>
    sleeping_loop_t;

using namespace tenacitas::logger::business;

struct thread_pool_098;
struct work_1
{
  work_1(thread_pool_098* p_098)
    : m_098(p_098)
  {}

  bool operator()(int32_t&& p_value);

private:
  thread_pool_098* m_098 = nullptr;
};

struct work_2
{
  work_2(thread_pool_098* p_098)
    : m_098(p_098)
  {}

  bool operator()(int32_t&& p_value);

private:
  thread_pool_098* m_098 = nullptr;
};

struct thread_pool_098
{
  thread_pool_098()
  {
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));

    m_pool.add_work(
      4, [this]() { return work_1(this); }, std::chrono::milliseconds(200));
  }

  bool operator()()
  {
    //    m_pool.add_work(
    //      4, [this]() { return work_1(this); },
    //      std::chrono::milliseconds(200));

    int32_t _value = 0;

    m_pool.run();

    m_pool.handle(_value);
    while (true) {
      std::unique_lock<std::mutex> _lock(m_mutex_stop);
      if (m_cond_stop.wait_for(_lock,
                               std::chrono::milliseconds(50),
                               [this]() -> bool { return m_stop; })) {
        concurrent_log_test(log, "notified");
        m_pool.stop();
        break;
      }
      //      if (m_stop) {
      //        m_pool.stop();
      //        break;
      //      }
      m_pool.handle(++_value);
    }
    concurrent_log_test(log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    concurrent_log_test(log, "woke up");
    return true;
  }

  void stop(const std::string& p_str)
  {
    concurrent_log_test(log, "stop called with ", p_str);
    m_stop = true;
    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_cond_stop.notify_all();
  }

private:
  bool m_stop = false;
  thread_pool_t m_pool;
  std::condition_variable m_cond_stop;
  std::mutex m_mutex_stop;
};

bool
work_1::operator()(int32_t&& p_value)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(170));
  concurrent_log_test(log, "work 1 handling msg ", p_value);
  if (p_value > 200) {
    m_098->stop("work1");
    return false;
  }
  return true;
}

bool
work_2::operator()(int32_t&& p_value)
{
  concurrent_log_test(log, "work 1 handling msg ", p_value);
  if (p_value > 100) {
    m_098->stop("work2");
    return false;
  }
  return true;
}

int
main(int argc, char** argv)
{
  tenacitas::logger::business::configure_cerr_log();
  run_test(thread_pool_098, argc, argv, "\nStopping when a max value is hit");
}
