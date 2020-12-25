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
#include <tuple>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <logger/cerr.h>
#include <tester/test.h>

using namespace tenacitas;

using namespace std::chrono_literals;

concurrent::timeout_callback _timeout_callback = [](std::thread::id p_id) {
  logger::cerr _log{"timeout_callback "};
  _log.set_debug_level();
  concurrent_warn(_log, "timeout for ", p_id);
};

struct async_loop_000 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, one param, no breaker";
    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    int16_t _i{0};
    auto _provider = [this, &_i]() -> int16_t {
      ++_i;
      concurrent_debug(m_log, "providing ", _i);
      return _i;
    };

    auto _worker = [this](int16_t p_i) -> void {
      concurrent_debug(m_log, "working with = ", p_i);
      std::this_thread::sleep_for(250ms);
    };

    auto _breaker = []() -> bool { return false; };

    concurrent::async_loop_t<logger::cerr, int16_t> _loop(
        500ms, _timeout_callback, _breaker, _worker, _provider);

    _loop.set_log_debug_level();

    _loop.start();

    std::this_thread::sleep_for(2s);

    _loop.stop();

    if (_i != 8) {
      concurrent_error(m_log, "i should be 8, but it is ", _i);
      return false;
    }

    concurrent_info(m_log, "i is 8, as expected");

    return true;
  }

private:
  logger::cerr m_log{"async_loop_000"};
};

struct async_loop_001 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, one param, breaker";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_001"};
};

struct async_loop_002 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, many params, no breaker";
    return _stream.str();
  }

  bool operator()() {

    typedef concurrent::async_loop_t<logger::cerr, int16_t, float> async_loop;

    auto _worker = [](int16_t p_i, float p_f) -> void {
      std::cout << p_i * p_f;
    };

    auto _provider = []() -> std::tuple<int16_t, float> { return {2, 3.5}; };

    async_loop _async_loop(
        1s, _timeout_callback, []() -> bool { return false; }, _worker,
        _provider);

    _async_loop.start();

    return true;
  }

private:
  logger::cerr m_log{"async_loop_002"};
};

struct async_loop_003 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, many params, breaker";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_003"};
};

struct async_loop_004 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, no params, no breaker";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_004"};
};

struct async_loop_005 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, no params, breaker";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_005"};
};

struct async_loop_006 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, one param";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_006"};
};

struct async_loop_007 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, many params";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_007"};
};

struct async_loop_008 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, no params";
    return _stream.str();
  }

  bool operator()() { return true; }

private:
  logger::cerr m_log{"async_loop_008"};
};

int main(int argc, char **argv) {
  //  logger::set_debug_level();

  tenacitas::tester::test _tester(argc, argv);
  run_test(_tester, async_loop_000);
  run_test(_tester, async_loop_001);
  run_test(_tester, async_loop_002);
  run_test(_tester, async_loop_003);
  run_test(_tester, async_loop_004);
  run_test(_tester, async_loop_005);
  run_test(_tester, async_loop_006);
  run_test(_tester, async_loop_007);
  run_test(_tester, async_loop_008);
}
