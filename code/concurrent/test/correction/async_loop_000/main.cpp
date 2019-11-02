#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <concurrent/business/internal/async_loop.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/thread.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

using namespace tenacitas::concurrent::business;
using namespace tenacitas::logger::business;

struct work1
{
  work_status operator()()
  {
    using namespace tenacitas;
    if (counter > 100) {
      return work_status::stop;
    }
    concurrent_log_test(logger::business::log, counter);
    ++counter;
    return work_status::dont_stop;
  }

  uint16_t counter = 0;
};

typedef async_loop_t<void, log> async_loop;

struct async_loop_000
{
  bool operator()()
  {
    using namespace tenacitas;
    bool _result = true;
    try {
      work1 _work;
      async_loop _async_loop(
        _work, std::chrono::milliseconds(100), []() -> work_status {
          return work_status::dont_stop;
        });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      concurrent_log_test(logger::business::log, "counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception& _ex) {
      concurrent_log_fatal(
        logger::business::log, "ERROR async_loop_000: '", _ex.what(), "'");
      return false;
    }
    return _result;
  }
};

int
main(int argc, char** argv)
{
  configure_cerr_log();
  run_test(
    async_loop_000,
    argc,
    argv,
    "\nWork in 'async_loop' will stop when a internal counter reaches 100."

    "\nMain thread will sleep for 4 secs."

    "\nCounter should be 101.")
}
