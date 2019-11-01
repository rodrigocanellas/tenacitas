
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
#include <logger/business/log.h>
#include <tester/business/run.h>

using namespace tenacitas;

struct work1
{
  concurrent::business::result operator()()
  {
    using namespace tenacitas;
    if (counter > 100) {
      return concurrent::business::result::stop;
    }
    concurrent_log_test(logger::business::log, counter);
    ++counter;
    return concurrent::business::result::dont_stop;
  }

  uint16_t counter = 0;
};

typedef concurrent::business::async_loop_t<void, logger::business::log> async_loop;

struct async_loop_000
{
  bool operator()()
  {
    using namespace tenacitas;
    bool _result = true;
    try {
      work1 _work;
      concurrent::business::traits_t<void>::worker _fc =
        [&_work]() -> concurrent::business::result { return _work(); };
      async_loop _async_loop(
        std::move(_fc),
        std::chrono::milliseconds(100),
        []() -> concurrent::business::result { return concurrent::business::result::dont_stop; });

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
  logger::business::configure_cerr_log();
  run_test(
    async_loop_000,
    argc,
    argv,
    "\nWork in 'async_loop' will stop when a internal counter reaches 100."

    "\nMain thread will sleep for 4 secs."

    "\nCounter should be 101.")
}
