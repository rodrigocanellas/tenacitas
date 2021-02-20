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

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct timeout_callback {
  timeout_callback(logger::cerr<> &p_log) : m_log(p_log) {}
  void operator()() { WAR(m_log, "TIMEOUT"); }

private:
  logger::cerr<> &m_log;
};

struct sleeping_loop_000 {

  static const std::string desc() { return "'sleeping_loop' creation test"; }

  bool operator()() {
    m_log.set_debug_level();

    typedef concurrent::sleeping_loop_t<logger::cerr<>, void> loop;

    auto _operation = [this]() -> void { DEB(m_log, "loop1"); };

    auto _on_timeout = []() -> void {};

    loop _loop(100ms, 1s, _operation, _on_timeout);

    return true;
  }

private:
  logger::cerr<> m_log{"sleeping_loop_000"};
};

struct sleeping_loop_001 {
  typedef concurrent::sleeping_loop_t<logger::cerr<>, void> loop;

  typedef uint16_t value;

  static const std::string desc() {
    std::stringstream _stream;
    _stream << "\n'sleeping_loop' with interval of " << m_interval_secs
            << "s, operation timeout of " << m_timeout
            << "ms, increments a counter, and just prints so there will be no "
               "timeout."

            << "\nCounter should be " << m_amount;
    return _stream.str();
  }

  bool operator()() {
    m_log.set_debug_level();

    auto _on_timeout = []() -> void {};

    operation1 _op(&m_cond);
    loop _loop(
        std::chrono::milliseconds(m_timeout),
        std::chrono::seconds(m_interval_secs), [&_op]() { return _op(); },
        _on_timeout);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_op]() -> bool { return _op.counter >= m_amount; });
    }

    _loop.stop();

    if (_op.counter != m_amount) {
      ERR(m_log, "counter should be ", m_amount, ", but it is ", _op.counter);
      return false;
    }

    INF(m_log, "counter should be ", m_amount, ", and it really is ",
        _op.counter);

    return true;
  }

private:
  struct operation1 {

    operation1(std::condition_variable *p_cond) : m_cond(p_cond) {}

    void operator()() {
      if (counter < m_amount) {
        ++counter;
        DEB(m_log, "counter = ", counter);
        std::this_thread::sleep_for(m_sleep);
      } else {
        m_cond->notify_one();
      }
    }

    value counter{0};

  private:
    logger::cerr<> m_log{"sleeping_loop_001::operation1"};
    std::condition_variable *m_cond;
  };

private:
  logger::cerr<> m_log{"sleeping_loop_001"};
  std::mutex m_mutex;
  std::condition_variable m_cond;

  static constexpr value m_interval_secs{1};
  static constexpr value m_amount{14};
  static constexpr value m_timeout{400};
  static constexpr std::chrono::milliseconds m_sleep{200};
};

// struct sleeping_loop_003 {

//  typedef concurrent::sleeping_loop_t<logger::cerr<>, void> loop;

//  static const std::string desc() {
//    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
//           "increments a counter, and just prints using 'cerr_test', so there
//           " "will be no timeout."

//           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10
//           " "secs; the 'sleeping_loop' will stop by calling 'stop'; the main
//           " "thread will sleep for 5 secs;, the 'sleeping_loop' will be "
//           "restarted with 'start'; the main thread will sleep for 8 secs; the
//           "
//           "'sleeping_loop' will stop in the destructor."

//           "\nCounter should be 18.";
//  }

//  bool operator()() {
//    m_log.set_debug_level();
//    using namespace tenacitas;

//    operation1 _op;

//    auto _on_timeout = []() -> void {};

//    loop _loop(
//        500ms, 1s, [&_op]() { return _op(); }, _on_timeout);

//    _loop.start();
//    std::this_thread::sleep_for(10s);
//    _loop.stop();

//    DEB(m_log, "data = ", _op.counter);
//    if (_op.counter != 10) {
//      return false;
//    }

//    std::this_thread::sleep_for(5s);

//    _loop.start();
//    std::this_thread::sleep_for(8s);

//    DEB(m_log, "data = ", _op.counter);
//    if (_op.counter != 18) {
//      return false;
//    }

//    return true;
//  }

// private:
//  struct operation1 {
//    void operator()() {

//      ++counter;
//      DEB(m_log, counter);
//    }
//    uint64_t counter = 0;
//    logger::cerr<> m_log{"sleeping_loop_003::operation1"};
//  };

// private:
//  logger::cerr<> m_log{"sleeping_loop_003"};
//};

// struct sleeping_loop_004 {
//  typedef concurrent::sleeping_loop_t<logger::cerr<>, void> loop;

//  static const std::string desc() {
//    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
//           "increments a counter, and just prints using 'cerr_test', so there
//           " "will be no timeout."

//           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10
//           " "secs; the 'sleeping_loop' will stop by calling 'stop'; the main
//           " "thread will sleep for 5 secs;, then 'sleeping_loop' will be "
//           "restarted with 'start'; the main thread will sleep for 8 secs; the
//           "
//           "'sleeping_loop' will stop by calling 'stop'."

//           "\nCounter should be 18.";
//  }

//  bool operator()() {
//    m_log.set_debug_level();

//    operation1 _operation;

//    auto _on_timeout = []() -> void {};

//    loop _loop(
//        500ms, 1s, [&_operation]() { return _operation(); }, _on_timeout);

//    _loop.start();
//    std::this_thread::sleep_for(10s);
//    _loop.stop();

//    DEB(m_log, "data = ", _operation.counter);
//    if (_operation.counter != 10) {
//      return false;
//    }

//    std::this_thread::sleep_for(5s);

//    _loop.start();
//    std::this_thread::sleep_for(8s);
//    _loop.stop();

//    DEB(m_log, "data = ", _operation.counter);
//    if (_operation.counter != 18) {
//      return false;
//    }
//    return true;
//  }

// private:
//  struct operation1 {
//    void operator()() {
//      using namespace tenacitas;
//      ++counter;
//      DEB(m_log, counter);
//    }
//    uint64_t counter = 0;
//    logger::cerr<> m_log{"sleeping_loop_004::operation1"};
//  };

// private:
//  logger::cerr<> m_log{"sleeping_loop_004"};
//};

// struct sleeping_loop_005 {
//  typedef concurrent::sleeping_loop_t<logger::cerr<>, void> loop;

//  static const std::string desc() {
//    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
//           "increments a counter, and just prints using 'cerr_test', so there
//           " "will be no timeout."

//           "\nA 'sleeping_loop' will be started, the main thread will sleep "
//           "for 10 secs, 'sleeping_loop' will be created using the same "
//           "parameters as the first one, the main thread will sleep for 3 "
//           "secs, the new 'sleeping_loop' will stop by its destructor."

//           "\nCounter should be 13.";
//  }

//  bool operator()() {

//    m_log.set_debug_level();

//    operation1 _operation;

//    auto _on_timeout = []() -> void {};

//    loop _loop_1(
//        500ms, 1s, [&_operation]() { return _operation(); }, _on_timeout);

//    _loop_1.start();
//    std::this_thread::sleep_for(10s);

//    if (_operation.counter != 10) {
//      ERR(m_log, "counter should be 10, but it is ", _operation.counter);
//      return false;
//    }
//    DEB(m_log, "data = ", _operation.counter);

//    _loop_1.stop();

//    loop _loop_2(
//        500ms, 1000ms, [&_operation]() { return _operation(); }, _on_timeout);

//    _loop_2.start();
//    std::this_thread::sleep_for(3s);

//    if (_operation.counter != 13) {
//      ERR(m_log, "counter should be 10, but it is ", _operation.counter);
//      return false;
//    }
//    DEB(m_log, "data = ", _operation.counter);
//    return true;
//  }

// private:
//  struct operation1 {
//    void operator()() {
//      using namespace tenacitas;
//      ++counter;
//      DEB(m_log, counter);
//    }
//    uint64_t counter = 0;
//    logger::cerr<> m_log{"sleeping_loop_005::operation1"};
//  };

// private:
//  logger::cerr<> m_log{"sleeping_loop_005"};
//};

// struct sleeping_loop_006 {

//  typedef concurrent::sleeping_loop_t<logger::cerr<>> loop;

//  static const std::string desc() {
//    return "\n'sleeping_loop' with interval of 1000 ms, timeout of 500 ms, "
//           "increments a counter, and just prints using 'cerr_test', so there
//           " "will be no timeout."

//           "\nA 'sleeping_loop' will be started, the main thread will sleep "
//           "for 10 secs, another 'sleeping_loop' will be created using the "
//           "same parameters as the first one, the main thread will sleep for 3
//           " "secs, the new 'sleeping_loop' will stop by calling stop."

//           "\nCounter should be 13.";
//  }

//  bool operator()() {
//    using namespace tenacitas;

//    std::function<void(std::thread::id)> _on_timeout =
//        [](std::thread::id) -> void {};

//    operation1 _operation;

//    loop _loop_1(
//        std::chrono::milliseconds(500), std::chrono::seconds(1),
//        [&_operation]() { return _operation(); }, _on_timeout);

//    _loop_1.start();
//    std::this_thread::sleep_for(std::chrono::seconds(10));

//    DEB(m_log, "data = ", _operation.counter);
//    if (_operation.counter != 10) {
//      ERR(m_log, "counter should be 10, but it is ",
//                       _operation.counter);
//      return false;
//    }

//    _loop_1.stop();

//    loop _loop_2(_loop_1.get_timeout(), _loop_1.get_interval(),
//                 _loop_1.get_operation(), _on_timeout);

//    _loop_2.start();

//    std::this_thread::sleep_for(std::chrono::seconds(3));

//    _loop_2.stop();

//    DEB(m_log, "data = ", _operation.counter);
//    if (_operation.counter != 13) {
//      ERR(m_log, "counter should be 13, but it is ",
//                       _operation.counter);
//      return false;
//    }

//    return true;
//  }

// private:
//  struct operation1 {
//    void operator()() {

//      ++counter;
//      DEB(m_log, counter);
//    }
//    uint64_t counter = 0;
//    logger::cerr<> m_log{"sleeping_loop_006::operation1"};
//  };

// private:
//  logger::cerr<> m_log{"sleeping_loop_006"};
//};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test<> _tester(argc, argv);

  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
  //  run_test(_tester, sleeping_loop_003);
  //  run_test(_tester, sleeping_loop_004);
  //  run_test(_tester, sleeping_loop_005);
  //  run_test(_tester, sleeping_loop_006);
}
