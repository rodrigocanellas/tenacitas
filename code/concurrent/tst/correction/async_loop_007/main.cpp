
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/bus/internal/async_loop.h>
#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

struct work1 {
  concurrent::bus::work_status operator()(uint32_t &&p_data) {

    data = p_data;
    concurrent_log_debug(logger::cerr::log, p_data);
    return concurrent::bus::work_status::dont_stop;
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

  uint32_t get_data() const { return m_data; }

private:
  uint32_t m_data = 1;
};

typedef concurrent::bus::async_loop_t<uint32_t, logger::cerr::log> async_loop;

struct async_loop_007 {
  bool operator()() {
    using namespace tenacitas;
    try {
      work1 _work;
      provide _provide;
      async_loop _async_loop_1(
          [&_work](uint32_t &&p_data) { return _work(std::move(p_data)); },
          std::chrono::milliseconds(1000),
          []() { return concurrent::bus::work_status::dont_stop; },
          [&_provide]() -> std::pair<bool, uint32_t> { return _provide(); });

      _async_loop_1.run();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop_1.stop();

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      if (_work.data != 19) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 19");
        return false;
      }

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      async_loop _async_loop_2(
          [&_work](uint32_t &&p_data) { return _work(std::move(p_data)); },
          _async_loop_1.get_timeout(), _async_loop_1.get_break(),
          [&_provide]() -> std::pair<bool, uint32_t> { return _provide(); });

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      _async_loop_2.run();

      std::this_thread::sleep_for(std::chrono::seconds(3));

      _async_loop_2.stop();

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      if (_work.data != 34) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 35");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string name() { return "async_loop_007"; }
  static std::string desc() {
    return "\nWork in 'async_loop' will print a provided uint32_t value."

           "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
           "sleep."

           "\nFirst 'async_loop' will run, main thread will sleep for 4 secs, "
           "then 'async_loop' will stop."

           "\nA second 'async_loop' is created, based on first one. "

           "\nMain thread sleeps for 2 secs, second 'async_loop' will run. "

           "\nMain thread sleeps for 3 secs, and second 'async_loop' stops. "

           "\nData in work should be 34.";
  }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<async_loop_007>(argc, argv);
}
