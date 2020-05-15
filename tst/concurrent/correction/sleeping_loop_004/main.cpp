
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct work1 {
  concurrent::work_status operator()() {
    using namespace tenacitas;
    ++counter;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

struct sleeping_loop_004 {
  bool operator()() {

    work1 _work;
    loop _loop(std::chrono::milliseconds(1000), [&_work]() { return _work(); },
               std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\n'sleeping_loop' will call 'run'; main thread will sleep for 10 "
           "secs; "
           "the 'sleeping_loop' will stop by calling 'stop'; the main thread "
           "will "
           "sleep for 5 secs;, the 'sleeping_loop' will be restarted with "
           "'run'; "
           "the main thread will sleep for 8 secs; the 'sleeping_loop' will "
           "stop by "
           "calling 'stop'."

           "\nCounter should be 18.";
  }

  static const std::string name() { return "sleeping_loop_004"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<sleeping_loop_004>(argc, argv);
}
