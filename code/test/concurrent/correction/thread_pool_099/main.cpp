/// \example
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/thread_pool.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

#include <chrono>

using namespace tenacitas::business;

typedef concurrent::thread_pool_t<int32_t, logger::log> thread_pool;
typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

struct work_1
{
  concurrent::result operator()(int32_t&& p_value)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(170));
    concurrent_log_test(logger::log, "work 1 handling msg ", p_value);
    if (p_value > 200) {
      concurrent_log_test(logger::log, "stopping this worker");
      return concurrent::result::stop;
    }
    return concurrent::result::dont_stop;
  }
};

struct thread_pool_099
{
  thread_pool_099()
  {
    m_pool.add_work(
      1, []() { return work_1(); }, std::chrono::milliseconds(200));
    m_pool.run();
  }

  bool operator()()
  {

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    int32_t _value = 199;
    m_pool.handle(_value);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    _value = 201;

    m_pool.handle(_value);

    concurrent_log_test(logger::log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_test(logger::log, "woke up");
    return true;
  }

private:
  thread_pool m_pool;
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
    thread_pool_099, argc, argv, "\nTesting thread waiting implementation");
}
