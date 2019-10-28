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

struct work_1
{
  bool operator()(int32_t&& p_value){
    std::this_thread::sleep_for(std::chrono::milliseconds(170));
    concurrent_log_test(log, "work 1 handling msg ", p_value);
    if (p_value > 200) {
      concurrent_log_test(log, "stopping this worker");
      return false;
    }
    return true;
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

    concurrent_log_test(log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_test(log, "woke up");
    return true;
  }

private:
  thread_pool_t m_pool;
};



int
main(int argc, char** argv)
{
  tenacitas::logger::business::configure_cerr_log();
  run_test(thread_pool_099, argc, argv, "\nTesting thread waiting implementation");
}
