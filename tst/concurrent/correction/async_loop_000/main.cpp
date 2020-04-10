#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <concurrent/internal/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct work {
  concurrent::_bus::work_status operator()() {

    if (counter > 100) {
      return concurrent::_bus::work_status::stop;
    }

    concurrent_log_debug(logger::cerr::log, "work = ", this,
                         ", counter = ", counter++);

    return concurrent::_bus::work_status::dont_stop;
  }

  uint16_t counter = 0;
};

typedef concurrent::_bus::async_loop_t<void, logger::cerr::log> async_loop;

struct async_loop_000 {

  bool operator()() {

    bool _result = true;
    try {

      work _work;
      concurrent_log_debug(logger::cerr::log, "work = ", this);
      async_loop _async_loop([&_work]() { return _work(); },
                             std::chrono::milliseconds(100),
                             []() -> concurrent::_bus::work_status {
                               return concurrent::_bus::work_status::dont_stop;
                             });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log,
                           "final counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, "ERROR async_loop_000: '",
                           _ex.what(), "'");
      return false;
    }
    return _result;
  }

  static std::string name() { return "async_loop_000"; }
  static std::string desc() {
    return "\nWork in 'async_loop' will stop when a internal counter reaches "
           "100."
           "\nMain thread will sleep for 4 secs."
           "\nCounter should be 101.";
  }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<async_loop_000>(argc, argv);
}
