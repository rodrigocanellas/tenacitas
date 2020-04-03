
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

struct sleeping_loop_000 {

  bool operator()() {

    typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
                 concurrent_log_debug(logger::cerr::log, "loop1");
                 return concurrent::_bus::work_status::dont_stop;
               },
               std::chrono::milliseconds(100));

    return true;
  }

  static const std::string desc() { return "'sleeping_loop' creation test"; }
  static const std::string name() { return "sleeping_loop_000"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<sleeping_loop_000>(argc, argv);
}
