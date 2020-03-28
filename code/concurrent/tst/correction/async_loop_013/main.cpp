
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/bus/internal/async_loop.h>
#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

struct work1 {

  concurrent::bus::work_status operator()() {

    using namespace tenacitas;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_debug(logger::cerr::log, "work = ", this,
                         ", counter = ", counter++);
    return concurrent::bus::work_status::dont_stop;
  }

  uint64_t counter = 0;
};

typedef concurrent::bus::async_loop_t<void, logger::cerr::log> async_loop;

struct async_loop_013 {
  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop(_work, std::chrono::milliseconds(1000),
                             []() -> concurrent::bus::work_status {
                               return concurrent::bus::work_status::dont_stop;
                             });
      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 0) {
        concurrent_log_debug(logger::cerr::log,
                             "wrong value for data, it should be 0, but it is ",
                             _work.counter);
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
           "\nThe work functor will not have its internal counter modified, as "
           "it was passed by copy to 'async_loop'";
  }

  static std::string name() { return "async_loop_013"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<async_loop_013>(argc, argv);
}
