
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

using namespace tenacitas::concurrent::business;

struct work1
{
  result operator()()
  {
    using namespace tenacitas::logger::business;
    if (counter > 100) {
      return result::stop;
    }
    concurrent_log_test(log, counter);
    ++counter;
    return result::dont_stop;
  }

  uint16_t counter = 0;
};

typedef tenacitas::concurrent::business::
  async_loop_t<void, tenacitas::logger::business::log>
    async_loop;

struct async_loop_000
{
  bool operator()()
  {
    using namespace tenacitas::logger::business;
    bool _result = true;
    try {
      work1 _work;
      traits_t<void>::worker _fc = [&_work]() -> result { return _work(); };
      async_loop _async_loop(std::move(_fc),
                             std::chrono::milliseconds(100),
                             []() -> result { return result::dont_stop; });

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      concurrent_log_test(log, "counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception& _ex) {
      concurrent_log_fatal(log, "ERROR async_loop_000: '", _ex.what(), "'");
      return false;
    }
    return _result;
  }
};

int
main(int argc, char** argv)
{
  tenacitas::logger::business::configure_cerr_log();
  run_test(
    async_loop_000,
    argc,
    argv,
    "\nWork in 'async_loop' will stop when a internal counter reaches 100."

    "\nMain thread will sleep for 4 secs."

    "\nCounter should be 101.")
}
