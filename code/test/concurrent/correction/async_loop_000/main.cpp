
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <business/concurrent/internal/async_loop.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/thread.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

struct work1
{
  concurrent::result operator()()
  {
    using namespace tenacitas::business;
    if (counter > 100) {
      return concurrent::result::stop;
    }
    concurrent_log_test(logger::log, counter);
    ++counter;
    return concurrent::result::dont_stop;
  }

  uint16_t counter = 0;
};

typedef concurrent::async_loop_t<void, logger::log> async_loop;

struct async_loop_000
{
  bool operator()()
  {
    using namespace tenacitas::business;
    bool _result = true;
    try {
      work1 _work;
      concurrent::traits_t<void>::worker _fc =
        [&_work]() -> concurrent::result { return _work(); };
      async_loop _async_loop(
        std::move(_fc),
        std::chrono::milliseconds(100),
        []() -> concurrent::result { return concurrent::result::dont_stop; });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      concurrent_log_test(logger::log, "counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception& _ex) {
      concurrent_log_fatal(
        logger::log, "ERROR async_loop_000: '", _ex.what(), "'");
      return false;
    }
    return _result;
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
    async_loop_000,
    argc,
    argv,
    "\nWork in 'async_loop' will stop when a internal counter reaches 100."

    "\nMain thread will sleep for 4 secs."

    "\nCounter should be 101.")
}
