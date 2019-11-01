
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <business/concurrent/traits.h>
#include <business/calendar/epoch.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;
using namespace tenacitas::business;

struct work1
{
  concurrent::result operator()()
  {
    using namespace tenacitas::business;
    ++counter;
    concurrent_log_test(logger::log, counter);
    return concurrent::result::dont_stop;
  }
  uint64_t counter = 0;
};

typedef concurrent::
sleeping_loop_t<void, logger::log>
loop;

struct sleeping_loop_004
{
  bool operator()()
  {

    work1 _work;
    loop _loop(std::chrono::milliseconds(1000),
               [&_work]() { return _work(); },
    std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_log_test(logger::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    _loop.stop();

    concurrent_log_test(logger::log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }
    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
        sleeping_loop_004,
        argc,
        argv,
        "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
        "increments a counter, and just prints using 'cerr_test', so there will "
        "be no time out."

        "\n'sleeping_loop' will call 'run'; main thread will sleep for 10 secs; "
        "the 'sleeping_loop' will stop by calling 'stop'; the main thread will "
        "sleep for 5 secs;, the 'sleeping_loop' will be restarted with 'run'; "
        "the main thread will sleep for 8 secs; the 'sleeping_loop' will stop by "
        "calling 'stop'."

        "\nCounter should be 18.");
}
