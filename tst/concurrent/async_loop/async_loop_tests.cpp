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

    concurrent::async_loop_t<logger::cerr, int16_t> _loop(
        500ms, _timeout_callback, _worker, _provider);

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

    auto _breaker = [&_i]() -> bool {
      if (_i == 3) {
        return true;
      }
      return false;
    };

    concurrent::async_loop_t<logger::cerr, int16_t> _loop(
        500ms, _timeout_callback, _breaker, _worker, _provider);

    _loop.set_log_debug_level();

    _loop.start();

    std::this_thread::sleep_for(2s);

    _loop.stop();

    if (_i != 3) {
      concurrent_error(m_log, "i should be 3, but it is ", _i);
      return false;
    }

    concurrent_info(m_log, "i is 3, as expected");

    return true;
  }

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

    m_log.set_debug_level();

    int16_t _i{0};
    float _f{0.0};
    auto _provider = [this, &_i, &_f]() -> std::tuple<int16_t, float> {
      ++_i;
      _f = _i * 2.5;
      concurrent_debug(m_log, "providing ", _i);
      return {_i, _f};
    };

    auto _worker = [this](int16_t p_i, float p_f) -> void {
      concurrent_debug(m_log, "working with = ", p_i, ", ", p_f);
      std::this_thread::sleep_for(250ms);
    };

    concurrent::async_loop_t<logger::cerr, int16_t, float> _loop(
        500ms, _timeout_callback, _worker, _provider);

    _loop.set_log_debug_level();

    _loop.start();

    std::this_thread::sleep_for(2s);

    _loop.stop();

    if ((_i != 8) && (_f != (8 * 2.5))) {
      concurrent_error(
          m_log, "i should be 8 and f should be 20.0, but they are they are ",
          _i, " and ", _f);
      return false;
    }

    concurrent_info(m_log, "i is 8 and f is 20, as expected");

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

  bool operator()() {

    m_log.set_debug_level();

    int16_t _i{0};
    float _f{0.0};
    auto _provider = [this, &_i, &_f]() -> std::tuple<int16_t, float> {
      ++_i;
      _f = _i * 2.5;
      concurrent_debug(m_log, "providing ", _i);
      return {_i, _f};
    };

    auto _worker = [this](int16_t p_i, float p_f) -> void {
      concurrent_debug(m_log, "working with = ", p_i, ", ", p_f);
      std::this_thread::sleep_for(250ms);
    };

    auto _breaker = [&_i]() -> bool {
      if (_i == 3) {
        return true;
      }
      return false;
    };

    concurrent::async_loop_t<logger::cerr, int16_t, float> _loop(
        500ms, _timeout_callback, _breaker, _worker, _provider);

    _loop.set_log_debug_level();

    _loop.start();

    std::this_thread::sleep_for(2s);

    _loop.stop();

    if ((_i != 3) && (_f != (3 * 2.5))) {
      concurrent_error(
          m_log, "i should be 8 and f should be 7.5, but they are they are ",
          _i, " and ", _f);
      return false;
    }

    concurrent_info(m_log, "i is 8 and f is 3.75, as expected");

    return true;
  }

private:
  logger::cerr m_log{"async_loop_003"};
};

struct async_loop_004 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, no params, no breaker";
    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    int16_t _i{0};

    auto _worker = [this, &_i]() -> void {
      concurrent_debug(m_log, "working with = ", ++_i);
      std::this_thread::sleep_for(250ms);
    };

    concurrent::async_loop_t<logger::cerr, void> _loop(500ms, _timeout_callback,
                                                       _worker);

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
  logger::cerr m_log{"async_loop_004"};
};

struct async_loop_005 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, no params, breaker";
    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    int16_t _i{0};

    auto _worker = [this, &_i]() -> void {
      concurrent_debug(m_log, "working with = ", ++_i);
      std::this_thread::sleep_for(250ms);
    };

    auto _breaker = [&_i]() -> bool {
      if (_i == 3) {
        return true;
      }
      return false;
    };

    concurrent::async_loop_t<logger::cerr, void> _loop(500ms, _timeout_callback,
                                                       _breaker, _worker);

    _loop.set_log_debug_level();

    _loop.start();

    std::this_thread::sleep_for(2s);

    _loop.stop();

    if (_i != 3) {
      concurrent_error(m_log, "i should be 3, but it is ", _i);
      return false;
    }

    concurrent_info(m_log, "i is 3, as expected");

    return true;
  }

private:
  logger::cerr m_log{"async_loop_005"};
};

struct async_loop_006 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, one param";
    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    auto _callback = [this](std::thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      m_cond.notify_one();
    };

    int16_t _i{0};
    auto _provider = [this, &_i]() -> int16_t {
      ++_i;
      concurrent_debug(m_log, "providing ", _i);
      return _i;
    };

    auto _worker = [this](int16_t p_i) -> void {
      concurrent_debug(m_log, "working with = ", p_i);
      if (p_i == 5) {
        concurrent_debug(m_log, "causing timeout");
        std::this_thread::sleep_for(1s);
        return;
      }
      std::this_thread::sleep_for(250ms);
    };

    concurrent::async_loop_t<logger::cerr, int16_t> _loop(500ms, _callback,
                                                          _worker, _provider);

    _loop.set_log_debug_level();

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait_for(_lock, 3s);
      _loop.stop();
    }

    if (_i != 5) {
      concurrent_error(m_log, "i should be 5, but it is ", _i);
      return false;
    }

    concurrent_info(m_log, "i is 5, as expected");

    return true;
  }

private:
  logger::cerr m_log{"async_loop_006"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
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
