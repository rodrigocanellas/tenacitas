
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;
using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> loop;

struct work1 {
  concurrent::_bus::work_status operator()() {
    using namespace tenacitas;
    ++counter;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::_bus::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

struct work2 {
  concurrent::_bus::work_status operator()() {
    counter += 100;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::_bus::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

struct work3 {
  concurrent::_bus::work_status operator()() {
    using namespace tenacitas;
    counter += 1000;
    concurrent_log_debug(logger::cerr::log, counter);
    return concurrent::_bus::work_status::dont_stop;
  }
  uint64_t counter = 0;
};

struct sleeping_loop_010 {

  bool operator()() {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100));

    work2 _work_2;
    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100));

    work3 _work_3;
    loop _loop_3(std::chrono::milliseconds(1000),
                 [&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100));

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_log_debug(logger::cerr::log, "stopping 2");
    _loop_2.stop();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_debug(logger::cerr::log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 9) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 9000) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 "
           "ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the "
           "third by 1000."

           "\nAll will be started, the main thread will sleep for 6 secs, the "
           "second 'sleeping_loop' will be stopped, the main thread will sleep "
           "for "
           "seconds all 'sleeping_loop' will stopped by theirs destructors."

           "\nCounter 1 should be 6, counter 2 should be 900, and counter 3 "
           "should "
           "be 6000.";
  }

  static const std::string name() { return "sleeping_loop_010"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<sleeping_loop_010>(argc, argv);
}
