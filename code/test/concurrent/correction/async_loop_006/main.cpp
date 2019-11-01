
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <business/concurrent/traits.h>
#include <business/concurrent/internal/async_loop.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/thread.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;
using namespace tenacitas::business;

struct work1
{

  concurrent::result operator()(uint32_t&& p_data)
  {

    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
    concurrent_log_test(logger::log, p_data);
    return concurrent::result::dont_stop;
  }
};

struct provide
{
  std::pair<bool, uint32_t> operator()()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (m_data > 200) {
      return std::make_pair(false, 0);
    }
    return std::make_pair(true, m_data++);
  }
  uint32_t m_data = 0;
};

typedef concurrent::
async_loop_t<uint32_t, logger::log>
async_loop;

struct async_loop_006
{
  bool operator()()
  {
    using namespace tenacitas::business;
    try {

      async_loop _async_loop(work1(),
                             std::chrono::milliseconds(1000),
                             []() { return concurrent::result::dont_stop; },
      provide());

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop.stop();

    } catch (std::exception& _ex) {
      concurrent_log_error(logger::log, _ex.what());
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
        async_loop_006,
        argc,
        argv,
        "\nWork in 'async_loop' will print a provided uint32_t value, with a "
        "1001 ms sleep, which will cause a timeout."

        "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
        "sleep."

        "\nMain thread will stop for 4 secs; 'async_loop' stops."

        "\n4 timeouts warnings will be printed")
}
