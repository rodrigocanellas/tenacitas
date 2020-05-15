/// \example
#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/thread_pool.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#include <chrono>

using namespace tenacitas;

typedef concurrent::thread_pool_t<int32_t, logger::cerr::log> thread_pool;
typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

using namespace tenacitas;

struct work_1 {
  concurrent::work_status operator()(int32_t &&p_value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ++counter;
    concurrent_log_debug(logger::cerr::log, "work 1 handling msg ", p_value);
    return concurrent::work_status::dont_stop;
  }
  int16_t counter = 0;
};

struct work_2 {
  concurrent::work_status operator()(int32_t &&p_value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    ++counter;
    concurrent_log_debug(logger::cerr::log, "work 2 handling msg ", p_value);
    return concurrent::work_status::dont_stop;
  }
  int16_t counter = 0;
};

struct thread_pool_097 {
  bool operator()() {
    work_1 _work_1;
    work_2 _work_2;

    {
      thread_pool _pool;
      int32_t _value = 0;
      int16_t _counter = 0;
      sleeping_loop _loop(
          std::chrono::milliseconds(100),
          [&_pool, &_value, &_counter]() -> concurrent::work_status {
            ++_value;
            ++_counter;
            _pool.handle(_value);
            return concurrent::work_status::dont_stop;
          },
          std::chrono::milliseconds(300));

      _pool.add_work(
          [&_work_1](int32_t &&p_val) -> concurrent::work_status {
            return _work_1(std::move(p_val));
          },
          std::chrono::milliseconds(200));
      _pool.add_work(
          [&_work_2](int32_t &&p_val) -> concurrent::work_status {
            return _work_2(std::move(p_val));
          },
          std::chrono::milliseconds(200));

      _loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(6));

      _loop.stop();

      concurrent_log_debug(logger::cerr::log, "counter = ", _counter);

      _pool.run();
    }
    concurrent_log_debug(logger::cerr::log,
                         "work 1 counter = ", _work_1.counter);
    concurrent_log_debug(logger::cerr::log,
                         "work 2 counter = ", _work_2.counter);
    return ((_work_1.counter + _work_2.counter) == 60);
  }

  static std::string desc() {
    return "\nA 'sleeping_loop' with interval of 100 ms, generating int32_t "
           "numbers to a 'thread_pool'."
           "\nThe 'thread_pool' has two workers, which just print number."
           "\nThe first work sleeps for 50 ms before printing, and the second "
           "one "
           "sleeps for 60 ms."
           "\nThe main thread runs the 'sleeping_loop', sleeps for 6 secs, "
           "stops "
           "the "
           "'sleeping_loop', and then starts the 'thread_pool'."
           "\nThe amount of values work 1 and work 2 handle must be 60";
  }

  static std::string name() { return "thread_pool_097"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<thread_pool_097>(argc, argv);
}
