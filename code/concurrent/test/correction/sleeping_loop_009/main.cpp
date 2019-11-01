
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/business/traits.h>
#include <calendar/business/epoch.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;
using namespace tenacitas;

typedef concurrent::business::
sleeping_loop_t<void, logger::business::log>
loop;

struct work1
{
  concurrent::business::result operator()()
  {
    using namespace tenacitas;
    ++counter;
    concurrent_log_test(logger::business::log, counter);
    return concurrent::business::result::dont_stop;
  }
  uint64_t counter = 0;
};

struct work2
{
  concurrent::business::result operator()()
  {

    counter += 100;
    concurrent_log_test(logger::business::log, counter);
    return concurrent::business::result::dont_stop;
  }
  uint64_t counter = 0;
};

struct work3
{
  concurrent::business::result operator()()
  {
    using namespace tenacitas;
    counter += 1000;
    concurrent_log_test(logger::business::log, counter);
    return concurrent::business::result::dont_stop;
  }
  uint64_t counter = 0;
};

struct sleeping_loop_009
{

  bool operator()()
  {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work_1]() { return _work_1(); },
    std::chrono::milliseconds(100));

    work2 _work_2;
    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work_2]() { return _work_2(); },
    std::chrono::milliseconds(100));

    work3 _work_3;
    loop _loop_3(std::chrono::milliseconds(1000),
                 [&_work_3]() { return _work_3(); },
    std::chrono::milliseconds(100));

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_log_test(logger::business::log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 6) {
      return false;
    }

    concurrent_log_test(logger::business::log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_log_test(logger::business::log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 6000) {
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
        sleeping_loop_009,
        argc,
        argv,
        "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of 500 "
        "ms."

        "\nThe first one increments a counter by 1; the second by 100, and the "
        "third by 1000."

        "\nAll will be started, the main thread will sleep for 6 secs, and the "
        "all 'sleeping_loop' will stopped by theirs destructors."

        "\nCounter 1 should be 6, counter 2 should be 600, and counter 3 should "
        "be 6000.");
}
