
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

    data = p_data;
    concurrent_log_debug(logger::cerr::log, p_data);
    return concurrent::_bus::work_status::dont_stop;
  }

  uint32_t data = 0;
};

struct provide {
  std::pair<bool, uint32_t> operator()() {
    if (m_data > 1000) {
      return {false, 0};
    }
    return {true, m_data++};
  }

private:
  uint32_t m_data = 0;
};

typedef concurrent::_bus::async_loop_t<uint32_t, logger::cerr::log> async_loop;

struct async_loop_003 {
  bool operator()() {
    using namespace tenacitas;

    try {
      work1 _work;
      async_loop _async_loop(
          [&_work](uint32_t &&p_data) -> concurrent::_bus::work_status {
            return _work(std::move(p_data));
          },
          std::chrono::milliseconds(1000),
          []() { return concurrent::_bus::work_status::dont_stop; }, provide());

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      _async_loop.stop();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      _async_loop.run();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log, "data = ", _work.data);
      if (_work.data != 1000) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 1000");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\nWork in 'async_loop' will print a provided uint32_t value at "
           "each 200 ms."

           "\nProvider will provide uint32_t values, until 1000."

           "\nMain thread will stop for 2 secs; 'async_loop' will stop; main "
           "thread will sleep for 1 sec; 'async_loop' will run again; main "
           "thread will sleep for 2 secs."

           "\nData in work should be 1000";
  }
  static std::string name() { return "async_loop_003"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();

  tester::test::run<async_loop_003>(argc, argv);
}
