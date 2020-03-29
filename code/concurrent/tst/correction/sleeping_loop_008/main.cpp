
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
using namespace tenacitas;

struct work1 {
  concurrent::bus::work_status operator()() {
    using namespace tenacitas;
    ++counter;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::bus::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

typedef concurrent::bus::sleeping_loop_t<void, logger::cerr::log> loop;

struct sleeping_loop_008 {
  bool operator()() {

    work1 _work;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop_1.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    loop _loop_2(_loop_1.get_interval(), _loop_1.get_work(),
                 _loop_1.get_timeout());

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    _loop_2.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 15) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 "
           "secs, the 'sleeping_loop' will be stopped, another 'sleeping_loop' "
           "will "
           "be created using the same parameters as first one, the new "
           "'sleeping_loop' will be started, the main thread will sleep for 5 "
           "secs, "
           "the new 'sleeping_loop' will be stopped by calling 'stop'."

           "\nCounter should be 15.";
  }
  static const std::string name() { return "sleeping_loop_008"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<sleeping_loop_008>(argc, argv);
}
