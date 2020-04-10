
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

struct work1 {
  concurrent::_bus::work_status operator()(uint32_t &&p_data) {
    data += p_data;
    concurrent_log_debug(logger::cerr::log, "1 -> ", data);
    return concurrent::_bus::work_status::dont_stop;
  }
  uint32_t data = 0;
};

struct work2 {
  concurrent::_bus::work_status operator()(uint32_t &&p_data) {
    using namespace tenacitas;
    data = p_data;
    concurrent_log_debug(logger::cerr::log, "2 -> ", data);
    return concurrent::_bus::work_status::dont_stop;
  }
  uint32_t data = 0;
};

struct provide {
  std::pair<bool, uint32_t> operator()() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if (m_data > 1000) {
      return std::make_pair(false, 0);
    }
    return std::make_pair(true, m_data++);
  }

private:
  uint32_t m_data = 0;
};

typedef concurrent::_bus::async_loop_t<uint32_t, logger::cerr::log> async_loop;

struct async_loop_012 {
  bool operator()() {
    using namespace tenacitas;
    try {
      provide _provide;
      work1 _work_1;
      work2 _work_2;

      async_loop _al_1(
          [&_work_1](uint32_t &&p_data) { return _work_1(std::move(p_data)); },
          std::chrono::milliseconds(1000),
          []() { return concurrent::_bus::work_status::dont_stop; },
          [&_provide]() -> std::pair<bool, uint32_t> { return _provide(); });

      async_loop _al_2(
          [&_work_2](uint32_t &&p_data) { return _work_2(std::move(p_data)); },
          std::chrono::milliseconds(1000),
          []() { return concurrent::_bus::work_status::dont_stop; },
          [&_provide]() -> std::pair<bool, uint32_t> { return _provide(); });

      _al_1.run();
      _al_2.run();

      std::this_thread::sleep_for(std::chrono::seconds(10));

      _al_1.stop();
      _al_2.stop();

      concurrent_log_debug(logger::cerr::log, "work 1 data = ", _work_1.data);
      concurrent_log_debug(logger::cerr::log, "work 2 data = ", _work_2.data);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\nWork 1 in 'async_loop' will print a provided uint32_t value, as "
           "well as Work 2."

           "\nProvider will provide incremente uint32_t values, to a Work 1 "
           "and Work 2 objects, until 1000, with a 200 ms sleep."

           "\nMain thread will run a 'sleeping_loop'; stop for 10 secs."

           "\nThe final data for Work 1 and Work 2 is unpredictable.";
  }

  static std::string name() { return "async_loop_012"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<async_loop_012>(argc, argv);
}
