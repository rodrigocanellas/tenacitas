
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <business/calendar/epoch.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;
using namespace tenacitas::business;

struct sleeping_loop_000
{

  bool operator()()
  {

    logger::configure_cerr_log();

    typedef concurrent::sleeping_loop_t<void, logger::log> loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
                 concurrent_log_test(logger::log, "loop1");
                 return concurrent::result::dont_stop;
               },
               std::chrono::milliseconds(100));

    return true;
  }
};

int
main(int argc, char** argv)
{
  run_test(sleeping_loop_000, argc, argv, "'sleeping_loop' creation test")
}
