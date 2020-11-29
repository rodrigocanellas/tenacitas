/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct operation_timeout_callback {
  operation_timeout_callback(logger::cerr::log &p_log) : m_log(p_log) {}
  void operator()(std::thread::id p_id) {
    concurrent_warn(m_log, "TIMEOUT for ", p_id);
  }

private:
  logger::cerr::log &m_log;
};

struct sleeping_loop_000 {

  static const std::string desc() { return "'sleeping_loop' creation test"; }

  bool operator()() {

    typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

    auto _operation = [this]() -> void { concurrent_debug(m_log, "loop1"); };

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    std::chrono::seconds _interval(1);
    std::chrono::milliseconds _timeout(100);

    concurrent_debug(m_log, "timeout = ", _timeout.count(),
                     ", interval = ", _interval.count());

    loop _loop(_timeout, _interval, _operation, _timeout_callback);

    return true;
  }

private:
  logger::cerr::log m_log{"sleeping_loop_000"};
};

struct sleeping_loop_001 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

  typedef uint16_t value;

  static const std::string desc() {
    std::stringstream _stream;
    _stream << "\n'sleeping_loop' with interval of " << m_interval_secs
            << "s, operation timeout of " << m_timeout
            << "ms, increments a counter, and just prints using 'cerr_test', "
            << "so there will be no timeout."

            << "\nThe main function will sleep for " << m_sleep_secs
            << "s, and the 'sleeping_loop' will stop in the destructor."

            << "\nCounter should be " << m_actual_amount;
    return _stream.str();
  }

  bool operator()() {

    concurrent_debug(m_log, "interval = ", m_interval_secs,
                     ", sleep = ", m_sleep_secs, ", amount = ", m_amount,
                     ", actual amount = ", m_actual_amount,
                     ", timeout = ", m_timeout);

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    operation1 _operation;
    loop _loop(
        std::chrono::milliseconds(m_timeout),
        std::chrono::seconds(m_interval_secs),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop.start();
    concurrent_debug(m_log, "starting to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(m_sleep_secs));
    concurrent_debug(m_log, "waking up");

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != m_actual_amount) {
      concurrent_error(m_log, "counter should be ", m_actual_amount,
                       ", but it is ", _operation.counter);
      return false;
    }

    concurrent_info(m_log, "counter should be ", m_actual_amount,
                    ", and it is ", _operation.counter);

    return true;
  }

private:
  struct operation1 {

    void operator()() {
      ++counter;
      concurrent_debug(m_log, "counter = ", counter);
    }

    value counter{0};

  private:
    logger::cerr::log m_log{"sleeping_loop_001::operation1"};
  };

private:
  logger::cerr::log m_log{"sleeping_loop_001"};

  static constexpr value m_sleep_secs{10};
  static constexpr value m_interval_secs{1};
  static constexpr float m_amount{
      static_cast<float>(m_sleep_secs / m_interval_secs)};
  static constexpr value m_actual_amount{
      static_cast<value>(((m_amount - static_cast<value>(m_amount)) != 0)
                             ? static_cast<value>(m_amount) + 1
                             : static_cast<value>(m_amount))};
  static constexpr value m_timeout{300};
};

struct sleeping_loop_003 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there  "
           "will be no timeout."

           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10 "
           "secs; the 'sleeping_loop' will stop by calling 'stop'; the main "
           "thread will sleep for 5 secs;, the 'sleeping_loop' will be "
           "restarted with 'start'; the main thread will sleep for 8 secs; the "
           "'sleeping_loop' will stop in the destructor."

           "\nCounter should be 18.";
  }

  bool operator()() {
    using namespace tenacitas;

    operation1 _operation;

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    loop _loop(
        std::chrono::milliseconds(500), std::chrono::seconds(1),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(8));

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 18) {
      return false;
    }

    return true;
  }

private:
  struct operation1 {
    void operator()() {

      ++counter;
      concurrent_debug(m_log, counter);
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_003::operation1"};
  };

private:
  logger::cerr::log m_log{"sleeping_loop_003"};
};

struct sleeping_loop_004 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no timeout."

           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10 "
           "secs; the 'sleeping_loop' will stop by calling 'stop'; the main "
           "thread will sleep for 5 secs;, then 'sleeping_loop' will be "
           "restarted with 'start'; the main thread will sleep for 8 secs; the "
           "'sleeping_loop' will stop by calling 'stop'."

           "\nCounter should be 18.";
  }

  bool operator()() {

    operation1 _operation;

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    loop _loop(
        std::chrono::milliseconds(500), std::chrono::seconds(1),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 18) {
      return false;
    }
    return true;
  }

private:
  struct operation1 {
    void operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_004::operation1"};
  };

private:
  logger::cerr::log m_log{"sleeping_loop_004"};
};

struct sleeping_loop_005 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no timeout."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, 'sleeping_loop' will be created using the same "
           "parameters as the first one, the main thread will sleep for 3 "
           "secs, the new 'sleeping_loop' will stop by its destructor."

           "\nCounter should be 13.";
  }

  bool operator()() {

    operation1 _operation;

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    loop _loop_1(
        std::chrono::milliseconds(500), std::chrono::seconds(1),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop_1.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (_operation.counter != 10) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _operation.counter);
      return false;
    }
    concurrent_debug(m_log, "data = ", _operation.counter);

    _loop_1.stop();

    loop _loop_2(
        std::chrono::milliseconds(500), std::chrono::milliseconds(1000),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop_2.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (_operation.counter != 13) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _operation.counter);
      return false;
    }
    concurrent_debug(m_log, "data = ", _operation.counter);
    return true;
  }

private:
  struct operation1 {
    void operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_005::operation1"};
  };

private:
  logger::cerr::log m_log{"sleeping_loop_005"};
};

struct sleeping_loop_006 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log> loop;

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no timeout."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, another 'sleeping_loop' will be created using the "
           "same parameters as the first one, the main thread will sleep for 3 "
           "secs, the new 'sleeping_loop' will stop by calling stop."

           "\nCounter should be 13.";
  }

  bool operator()() {
    using namespace tenacitas;

    std::function<void(std::thread::id)> _timeout_callback =
        [](std::thread::id) -> void {};

    operation1 _operation;

    loop _loop_1(
        std::chrono::milliseconds(500), std::chrono::seconds(1),
        [&_operation]() { return _operation(); }, _timeout_callback);

    _loop_1.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 10) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _operation.counter);
      return false;
    }

    _loop_1.stop();

    loop _loop_2(_loop_1.get_timeout(), _loop_1.get_interval(),
                 _loop_1.get_operation(), _timeout_callback);

    _loop_2.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    _loop_2.stop();

    concurrent_debug(m_log, "data = ", _operation.counter);
    if (_operation.counter != 13) {
      concurrent_error(m_log, "counter should be 13, but it is ",
                       _operation.counter);
      return false;
    }

    return true;
  }

private:
  struct operation1 {
    void operator()() {

      ++counter;
      concurrent_debug(m_log, counter);
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_006::operation1"};
  };

private:
  logger::cerr::log m_log{"sleeping_loop_006"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _tester(argc, argv);

  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
  run_test(_tester, sleeping_loop_003);
  run_test(_tester, sleeping_loop_004);
  run_test(_tester, sleeping_loop_005);
  run_test(_tester, sleeping_loop_006);
}
