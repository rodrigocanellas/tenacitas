
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <calendar/bus/epoch.h>
#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>



using namespace tenacitas;
using namespace tenacitas;

struct sleeping_loop_000
{

  bool operator()()
  {

    logger::bus::configure_cerr_log();

    typedef concurrent::bus::sleeping_loop_t<void, logger::cerr::log> loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
                 concurrent_log( "loop1");
                 return concurrent::bus::work_status::dont_stop;
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
