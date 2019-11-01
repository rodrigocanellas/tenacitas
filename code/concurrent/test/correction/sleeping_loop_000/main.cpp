
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <calendar/business/epoch.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

using namespace tenacitas;
using namespace tenacitas;

struct sleeping_loop_000
{

  bool operator()()
  {

    logger::business::configure_cerr_log();

    typedef concurrent::business::sleeping_loop_t<void, logger::business::log> loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
                 concurrent_log_test(logger::business::log, "loop1");
                 return concurrent::business::result::dont_stop;
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
