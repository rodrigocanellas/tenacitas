/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct async_loop_000 {
  bool operator()() {
    using namespace tenacitas;
    using namespace std;
    typedef concurrent::async_loop_t<logger::cerr::log, chrono::milliseconds>
        async_loop;

    uint16_t _counter = 0;
    function<bool()> _breaker = [&_counter]() -> bool {
      if (_counter == max) {
        return true;
      }
      ++_counter;
      return false;
    };

    function<void()> _worker = [this, &_counter]() -> void {
      concurrent_debug(m_log, "counter = ", _counter);
    };

    async_loop _async_loop(chrono::milliseconds(100), _worker, _breaker);

    _async_loop.start();

    concurrent_debug(m_log, "going to sleep");
    this_thread::sleep_for(chrono::seconds(1));
    concurrent_debug(m_log, "waking up");

    concurrent_debug(m_log, "loop stopped and counter = ", _counter);
    return (_counter == max);
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: no; breaker: yes; stop by breaker: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_000"};
  static constexpr uint16_t max{50};
};

struct async_loop_001 {
  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::async_loop_t<logger::cerr::log, chrono::milliseconds>
        async_loop;

    bool _is_timeout{false};
    std::function<bool()> _breaker = [&_is_timeout]() -> bool {
      return _is_timeout;
    };

    uint16_t _counter = 0;
    std::function<void()> _worker = [this, &_counter]() -> void {
      if (_counter >= static_cast<uint16_t>(max / 2)) {
        concurrent_debug(m_log, "sleeping enough for timeout");
        this_thread::sleep_for(chrono::milliseconds(2 * timeout));
        concurrent_debug(m_log, "waking from sleep enough for timeout");
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    std::function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "TIMEOUT for ", p_id);
      _is_timeout = true;
    };

    async_loop _async_loop(std::chrono::milliseconds(timeout), _worker,
                           _breaker, _timeout_callback);

    _async_loop.start();

    this_thread::sleep_for(chrono::milliseconds(3 * timeout));

    concurrent_debug(m_log, "timeout? ", (_is_timeout ? "yes" : "no"));

    if (_is_timeout) {
      _async_loop.stop();
    }

    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: no; breaker: yes; stop by breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_001"};
  static constexpr uint16_t max{50};
  static constexpr uint16_t timeout{3000};
};

struct async_loop_002 {
  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::async_loop_t<logger::cerr::log, chrono::milliseconds>
        async_loop;

    bool _is_timeout{false};
    std::function<bool()> _breaker = [&_is_timeout]() -> bool {
      return _is_timeout;
    };

    uint16_t _counter = 0;
    std::function<void()> _worker = [this, &_counter]() -> void {
      this_thread::sleep_for(chrono::milliseconds(timeout / 2));
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    std::function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "TIMEOUT for ", p_id);
      _is_timeout = true;
    };

    async_loop _async_loop(std::chrono::milliseconds(timeout), _worker,
                           _breaker, _timeout_callback);

    _async_loop.start();

    this_thread::sleep_for(chrono::milliseconds(3 * timeout));

    concurrent_debug(m_log, "timeout? ", (_is_timeout ? "yes" : "no"));

    if (_is_timeout) {
      _async_loop.stop();
    }

    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: no; breaker: yes; stop by breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_002"};
  static constexpr uint16_t max{50};
  static constexpr uint16_t timeout{3000};
};

struct async_loop_003 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: no; breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_003"};
};

struct async_loop_004 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: no; breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_004"};
};

struct async_loop_005 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: one; breaker: yes; stop by breaker: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_005"};
};

struct async_loop_006 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "parameter: one; breaker: yes; stop by breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_006"};
};

struct async_loop_007 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: one; breaker: yes; stop by breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_007"};
};

struct async_loop_008 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: one; breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_008"};
};

struct async_loop_009 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: one; breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_009"};
};

struct async_loop_010 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: many; breaker: yes; stop by breaker: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_010"};
};

struct async_loop_011 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "parameter: many; breaker: yes; stop by breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_011"};
};

struct async_loop_012 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "parameter: many; breaker: yes; stop by breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_012"};
};

struct async_loop_013 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: many; breaker: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_013"};
};

struct async_loop_014 {
  bool operator()() { return true; }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "parameter: many; breaker: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_014"};
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();

  tenacitas::tester::test _tester(argc, argv);

  run_test(_tester, async_loop_000);
  run_test(_tester, async_loop_001);
  run_test(_tester, async_loop_002);
}
