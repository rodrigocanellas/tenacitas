/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct async_loop_000 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, no params, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, chrono::milliseconds,
                                     true>
        async_loop;

    bool _stop{false};
    uint16_t _counter{0};

    function<bool()> _breaker = [this, &_stop]() -> bool {
      concurrent_debug(m_log, "breaker will return ", (_stop ? "t" : "f"));
      m_cond.notify_one();
      return _stop;
    };

    function<void()> _worker = [this, &_counter]() -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "wroker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_stop](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _stop = true;
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _breaker,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock, [&_stop]() -> bool { return _stop; });

    return true;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_000"};

  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_001 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, no params, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_001"};
};

struct async_loop_002 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, one param, timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_002"};
};

struct async_loop_003 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, one param, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_003"};
};

struct async_loop_004 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, many params, timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_004"};
};

struct async_loop_005 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, many params, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_005"};
};

// #######################################

struct async_loop_006 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, no params, timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_006"};
};

struct async_loop_007 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, no params, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_007"};
};

struct async_loop_008 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, one param, timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_008"};
};

struct async_loop_009 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, one param, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_009"};
};

struct async_loop_010 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, many params, timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_010"};
};

struct async_loop_011 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, many params, no timeout";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_011"};
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();

  tenacitas::tester::test _tester(argc, argv);
  run_test(_tester, async_loop_000);
}
