
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

struct work1
{

  concurrent::business::result operator()()
  {

    using namespace tenacitas::logger::business;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    concurrent_log_test(log, counter);
    ++counter;
    return concurrent::business::result::dont_stop;;
  }

  uint64_t counter = 0;
};

typedef tenacitas::concurrent::business::
async_loop_t<void, tenacitas::logger::business::log>
async_loop;

struct async_loop_001
{
  bool operator()()
  {
    using namespace tenacitas::logger::business;
    using namespace tenacitas::concurrent::business;
    try {
      work1 _work;
      async_loop _async_loop([&_work]() -> result { return _work(); },
                               std::chrono::milliseconds(1000),
                               []() -> result { return result::dont_stop; });
      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_test(log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_test(log,
                            "wrong value for data, it should be 4");
        return false;
      }
    } catch (std::exception& _ex) {
      concurrent_log_error(log, _ex.what());
      return false;
    }

    return true;
  }
};

int
main(int argc, char** argv)
{
  tenacitas::logger::business::configure_cerr_log();
  run_test(
        async_loop_001,
        argc,
        argv,
        "\nWork in 'async_loop' will increment an internal counter indefinetly, "
        "at each 200 ms."

        "\nMain thread will stop for 1 sec."

        "\nCounter should be 4.")
}
