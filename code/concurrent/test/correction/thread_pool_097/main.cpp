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
    bool operator()(int32_t&& p_value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ++counter;
        concurrent_log_test(log, "work 1 handling msg ", p_value);
        return true;
    }
    int16_t counter = 0;
};

struct work_2
{
    bool operator()(int32_t&& p_value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
        ++counter;
        concurrent_log_test(log, "work 2 handling msg ", p_value);
        return true;
    }
    int16_t counter = 0;
};

struct thread_pool_097
{
    bool operator()()
    {
        work_1 _work_1;
        work_2 _work_2;

        {
            thread_pool_t _pool;
            int32_t _value = 0;
            int16_t _counter = 0;
            sleeping_loop_t _loop(std::chrono::milliseconds(100),
                                  [&_pool, &_value, &_counter]() -> bool {
                                      ++_value;
                                      ++_counter;
                                      _pool.handle(_value);
                                      return true;
                                  },
                                  std::chrono::milliseconds(300));

            _pool.add_work(
              [&_work_1](int32_t&& p_val) -> bool {
                  return _work_1(std::move(p_val));
              },
              std::chrono::milliseconds(200));
            _pool.add_work(
              [&_work_2](int32_t&& p_val) -> bool {
                  return _work_2(std::move(p_val));
              },
              std::chrono::milliseconds(200));

            _loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(6));

            _loop.stop();

            concurrent_log_test(log, "counter = ", _counter);

            _pool.run();
        }
        concurrent_log_test(log, "work 1 counter = ", _work_1.counter);
        concurrent_log_test(log, "work 2 counter = ", _work_2.counter);
        return ((_work_1.counter + _work_2.counter) == 60);
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(
      thread_pool_097,
      argc,
      argv,
      "\nA 'sleeping_loop' with interval of 100 ms, generating int32_t "
      "numbers to a 'thread_pool'."
      "\nThe 'thread_pool' has two workers, which just print number."
      "\nThe first work sleeps for 50 ms before printing, and the second one "
      "sleeps for 60 ms."
      "\nThe main thread runs the 'sleeping_loop', sleeps for 6 secs, stops "
      "the "
      "'sleeping_loop', and then starts the 'thread_pool'."
      "\nThe amount of values work 1 and work 2 handle must be 60");
}
