/// \example
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/concurrent/thread_pool.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

#include <chrono>

using namespace tenacitas::business;
using namespace tenacitas::business;

typedef concurrent::
thread_pool_t<int32_t, logger::log>
thread_pool;
typedef concurrent::
sleeping_loop_t<void, logger::log>
sleeping_loop;

using namespace tenacitas::business;

struct work_1
{
  concurrent::result operator()(int32_t&& p_value)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ++counter;
    concurrent_log_test(logger::log, "work 1 handling msg ", p_value);
    return concurrent::result::dont_stop;
  }
  int16_t counter = 0;
};

struct work_2
{
  concurrent::result operator()(int32_t&& p_value)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    ++counter;
    concurrent_log_test(logger::log, "work 2 handling msg ", p_value);
    return concurrent::result::dont_stop;
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
      thread_pool _pool;
      int32_t _value = 0;
      int16_t _counter = 0;
      sleeping_loop _loop(std::chrono::milliseconds(100),
                          [&_pool, &_value, &_counter]() -> concurrent::result {
        ++_value;
        ++_counter;
        _pool.handle(_value);
        return concurrent::result::dont_stop;
      },
      std::chrono::milliseconds(300));

      _pool.add_work(
            [&_work_1](int32_t&& p_val) -> concurrent::result {
        return _work_1(std::move(p_val));
      },
      std::chrono::milliseconds(200));
      _pool.add_work(
            [&_work_2](int32_t&& p_val) -> concurrent::result {
        return _work_2(std::move(p_val));
      },
      std::chrono::milliseconds(200));

      _loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(6));

      _loop.stop();

      concurrent_log_test(logger::log, "counter = ", _counter);

      _pool.run();
    }
    concurrent_log_test(logger::log, "work 1 counter = ", _work_1.counter);
    concurrent_log_test(logger::log, "work 2 counter = ", _work_2.counter);
    return ((_work_1.counter + _work_2.counter) == 60);
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
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
