
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/traits.h>
#include <concurrent/business/internal/async_loop.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/thread.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;
using namespace tenacitas;
struct work1
{

  concurrent::business::result operator()()
  {

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_test(logger::business::log, counter);
    ++counter;
    return concurrent::business::result::dont_stop;
  }

  uint64_t counter = 0;
};

typedef concurrent::business::
async_loop_t<void, logger::business::log>
async_loop;

struct async_loop_002
{
  bool operator()()
  {
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop([&_work]() -> concurrent::business::result { return _work(); },
                             std::chrono::milliseconds(1000),
                             []() -> concurrent::business::result { return concurrent::business::result::dont_stop; });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_test(logger::business::log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_error(logger::business::log,
                             "wrong value for data, it should be 4");
        return false;
      }

      _async_loop.stop();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_test(logger::business::log, "counter = ", _work.counter);
      if (_work.counter != 14) {
        concurrent_log_error(logger::business::log,
                             "wrong value for data, it should be 14");
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
        async_loop_002,
        argc,
        argv,
        "\nWork in 'async_loop' will increment an internal counter indefinetly, "
        "with a 200ms sleep."

        "\nMain thread will stop for 1 secs; counter should be 4; 'async_loop' "
        "will stop; main thread will sleep for 2 secs; 'async_loop' will run "
        "again; counter should be 14")
}
