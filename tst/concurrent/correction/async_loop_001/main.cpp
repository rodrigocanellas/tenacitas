
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/internal/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct work1 {

  concurrent::_bus::work_status operator()() {

    using namespace tenacitas;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_debug(logger::cerr::log, "work = ", this,
                         ", counter = ", counter++);
    return concurrent::_bus::work_status::dont_stop;
  }

  uint64_t counter = 0;
};

typedef concurrent::_bus::async_loop_t<void, logger::cerr::log> async_loop;

struct async_loop_001 {
  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop(
          [&_work]() -> concurrent::_bus::work_status { return _work(); },
          std::chrono::milliseconds(1000),
          []() -> concurrent::_bus::work_status {
            return concurrent::_bus::work_status::dont_stop;
          });
      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_debug(logger::cerr::log,
                             "wrong value for data, it should be 4");
        return false;
      }
    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\nWork in 'async_loop' will increment an internal counter "
           "indefinetly, at each 200 ms."
           "\nMain thread will stop for 1 sec."
           "\nCounter should be 4.";
  }

  static std::string name() { return "async_loop_001"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<async_loop_001>(argc, argv);
}
