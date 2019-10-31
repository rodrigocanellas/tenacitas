
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
#include <logger/business/log.h>
#include <tester/business/run.h>

using namespace tenacitas::concurrent::business;
using namespace tenacitas::logger::business;

struct sleeping_loop_000
{

  bool operator()()
  {

    configure_cerr_log();

    typedef tenacitas::concurrent::business::sleeping_loop_t<void, log>
        loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
      concurrent_log_test(log, "loop1");
      return result::dont_stop;
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
