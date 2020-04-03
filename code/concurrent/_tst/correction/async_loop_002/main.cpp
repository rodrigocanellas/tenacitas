
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/_bus/internal/async_loop.h>
#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/thread.h>
#include <concurrent/_bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

struct work1 {

  concurrent::_bus::work_status operator()() {

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_debug(logger::cerr::log, counter);
    ++counter;
    return concurrent::_bus::work_status::dont_stop;
  }

  uint64_t counter = 0;
};

typedef concurrent::_bus::async_loop_t<void, logger::cerr::log> async_loop;

struct async_loop_002 {
  bool operator()() {
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
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 4");
        return false;
      }

      _async_loop.stop();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 14) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 14");
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
           "indefinetly, "
           "with a 200ms sleep."

           "\nMain thread will stop for 1 secs; counter should be 4; "
           "'async_loop' "
           "will stop; main thread will sleep for 2 secs; 'async_loop' will "
           "run "
           "again; counter should be 14";
  }
  static std::string name() { return "async_loop_002"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<async_loop_002>(argc, argv);
}
