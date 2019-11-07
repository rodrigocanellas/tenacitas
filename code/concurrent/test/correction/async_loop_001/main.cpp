
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/internal/async_loop.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/thread.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;

struct work1
{

  concurrent::business::work_status operator()()
  {

    using namespace tenacitas;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_test(logger::business::log, counter);
    ++counter;
    return concurrent::business::work_status::dont_stop;
    ;
  }

  uint64_t counter = 0;
};

typedef concurrent::business::async_loop_t<void, logger::business::log> async_loop;

struct async_loop_001
{
  bool operator()()
  {
    using namespace tenacitas;
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop(
        [&_work]() -> concurrent::business::work_status { return _work(); },
        std::chrono::milliseconds(1000),
        []() -> concurrent::business::work_status { return concurrent::business::work_status::dont_stop; });
      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_test(logger::business::log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_test(logger::business::log,
                            "wrong value for data, it should be 4");
        return false;
      }
    } catch (std::exception& _ex) {
      concurrent_log_error(logger::business::log, _ex.what());
      return false;
    }

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::business::configure_cerr_log();
  run_test(
    async_loop_001,
    argc,
    argv,
    "\nWork in 'async_loop' will increment an internal counter indefinetly, "
    "at each 200 ms."

    "\nMain thread will stop for 1 sec."

    "\nCounter should be 4.")
}
