
/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

typedef concurrent::bus::sleeping_loop_t<void, logger::cerr::log> loop;

struct work1 {
  concurrent::bus::work_status operator()() {

    ++counter;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::bus::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

struct sleeping_loop_002 {

  bool operator()() {
    using namespace tenacitas;
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

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, work time out of 500 "
           "ms, increments a counter, and just prints using 'cerr_test', so "
           "there will be no time out."

           "\nThe main function will sleep for 10 secs, and the "
           "'sleeping_loop' will stop by calling 'stop'."

           "\nWork counter must be 10";
  }

  static const std::string name() { return "sleeping_loop_002"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<sleeping_loop_002>(argc, argv);
}
