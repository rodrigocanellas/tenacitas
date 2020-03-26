#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <concurrent/bus/internal/async_loop.h>
#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/traits.h>
#include <concurrent/tst/concurrent_log.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;
using namespace tenacitas::logger;

struct work1 {
  concurrent::bus::work_status operator()() {
    using namespace tenacitas;
    if (counter > 100) {
      return concurrent::bus::work_status::stop;
    }
    concurrent_log(counter);

    ++counter;
    return concurrent::bus::work_status::dont_stop;
  }

  uint16_t counter = 0;
};

typedef concurrent::bus::async_loop_t<void, tenacitas::logger::cerr::log>
    async_loop;

struct async_loop_000 {
  bool operator()() {
    using namespace tenacitas;
    bool _result = true;
    try {
      work1 _work;
      async_loop _async_loop(_work, std::chrono::milliseconds(100),
                             []() -> concurrent::bus::work_status {
                               return concurrent::bus::work_status::dont_stop;
                             });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      concurrent_log("counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception &_ex) {
      concurrent_log("ERROR async_loop_000: '", _ex.what(), "'");
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
  tester::test::run<async_loop_000>(argc, argv);
}
