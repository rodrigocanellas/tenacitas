
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/internal/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;
using namespace tenacitas;

struct work1 {

  concurrent::_bus::work_status operator()(uint32_t &&p_data) {

    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
    concurrent_log_debug(logger::cerr::log, p_data);
    return concurrent::_bus::work_status::dont_stop;
  }
};

struct provide {
  std::pair<bool, uint32_t> operator()() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (m_data > 200) {
      return std::make_pair(false, 0);
    }
    return std::make_pair(true, m_data++);
  }
  uint32_t m_data = 0;
};

typedef concurrent::_bus::async_loop_t<uint32_t, logger::cerr::log> async_loop;

struct async_loop_006 {
  bool operator()() {
    using namespace tenacitas;
    try {

      async_loop _async_loop(
          work1(), std::chrono::milliseconds(1000),
          []() { return concurrent::_bus::work_status::dont_stop; }, provide());

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop.stop();

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string name() { return "async_loop_006"; }
  static std::string desc() {
    return "\nWork in 'async_loop' will print a provided uint32_t value, with "
           "a 1001 ms sleep, which will cause a timeout. "
           "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
           "sleep. "
           "\nMain thread will stop for 4 secs; 'async_loop' stops.\n4 "
           "timeouts warnings will be printed";
  }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<async_loop_006>(argc, argv);
}