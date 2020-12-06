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
#include <logger/cerr/log.h>
#include <tester/test.h>

/// \brief Tests \p async_loop_t that uses a breaker, when the worker takes
/// no parameters, and at some point it takes more time than defined
struct async_loop_000 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, no params, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true> async_loop;

    bool _is_timeout{false};
    uint16_t _counter{0};

    function<bool()> _breaker = [this, &_is_timeout]() -> bool {
      concurrent_debug(m_log, "breaker will return ",
                       (_is_timeout ? "t" : "f"));
      m_cond.notify_one();
      return _is_timeout;
    };

    function<void()> _worker = [this, &_counter]() -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _breaker,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock, [&_is_timeout]() -> bool { return _is_timeout; });

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return _is_timeout;
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

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true> async_loop;

    concurrent_debug(m_log, "starting async_loop_001");

    uint16_t _counter{0};

    function<bool()> _breaker = [this, &_counter]() -> bool {
      if (_counter == m_max) {
        concurrent_debug(m_log, "breaking because counter = ", _counter);
        m_cond.notify_one();
        return true;
      }
      return false;
    };

    function<void()> _worker = [this, &_counter]() -> void {
      this_thread::sleep_for(chrono::seconds(1));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    concurrent_debug(m_log, "creating async");
    async_loop _async_loop(_worker, _breaker);
    concurrent_debug(m_log, "async created");

    concurrent_debug(m_log, "stating async");
    _async_loop.start();
    concurrent_debug(m_log, "async started");

    concurrent_debug(m_log, "waiting for the notification the loop stopped");
    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);
    concurrent_debug(m_log, "notification that loop stopped arrived");

    if (_counter == m_max) {
      concurrent_debug(m_log, "success");
    } else {
      concurrent_error(m_log, "fail");
    }

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_001"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{1};
};

struct async_loop_002 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, one param, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true, int32_t>
        async_loop;

    uint16_t _counter{0};
    bool _is_timeout{false};

    function<bool()> _breaker = [this, &_is_timeout]() -> bool {
      if (_is_timeout) {
        m_cond.notify_one();
        return true;
      }
      return false;
    };

    function<void(int32_t)> _worker = [this,
                                       &_counter](int32_t p_value) -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " value = ", p_value);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
    };

    int32_t _value{0};
    function<optional<int32_t>()> _provider = [this,
                                               &_value]() -> optional<int32_t> {
      _value += 30;
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _breaker,
                           _provider, _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return _is_timeout;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_002"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_003 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, one param, no timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true, int32_t>
        async_loop;

    uint16_t _counter{0};

    function<bool()> _breaker = [this, &_counter]() -> bool {
      if (_counter == m_max) {
        concurrent_debug(m_log, "breaking because counter = ", _counter);
        m_cond.notify_one();
        return true;
      }
      return false;
    };

    function<void(int32_t)> _worker = [this,
                                       &_counter](int32_t p_value) -> void {
      this_thread::sleep_for(chrono::seconds(1));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " value = ", p_value);
    };

    function<void(thread::id)> _timeout_callback =
        [this](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    int32_t _value{0};
    function<optional<int32_t>()> _provider = [this,
                                               &_value]() -> optional<int32_t> {
      _value += 30;
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(_worker, _breaker, _provider);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_003"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{10};
};

struct async_loop_004 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, many params, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true, int32_t, double,
                                     string>
        async_loop;

    uint16_t _counter{0};
    bool _is_timeout{false};

    function<bool()> _breaker = [this, &_is_timeout]() -> bool {
      if (_is_timeout) {
        m_cond.notify_one();
        return true;
      }
      return false;
    };

    function<void(int32_t, double, string)> _worker =
        [this, &_counter](int32_t p_int, double p_double,
                          string p_str) -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " int = ", p_int,
                       " double = ", p_double, ", string = ", p_str);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
    };

    int32_t _int{0};
    double _double{3.14};

    typedef tuple<int32_t, double, string> value;

    function<optional<value>()> _provider =
        [this, &_int, &_double]() -> optional<tuple<int32_t, double, string>> {
      _int += 30;
      _double += _int;
      string _str{"int & double = " + to_string(_int) + " " +
                  to_string(_double)};
      value _value{_int, _double, _str};
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _breaker,
                           _provider, _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return _is_timeout;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_004"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_005 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "breaker, many params, no timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, true, float, int16_t,
                                     double>
        async_loop;

    uint16_t _counter{0};

    function<bool()> _breaker = [this, &_counter]() -> bool {
      if (_counter == m_max) {
        concurrent_debug(m_log, "breaking because counter = ", _counter);
        m_cond.notify_one();
        return true;
      }
      return false;
    };

    function<void(float, int16_t, double)> _worker =
        [this, &_counter](float p_float, int16_t p_int,
                          double p_double) -> void {
      this_thread::sleep_for(
          chrono::milliseconds(static_cast<uint16_t>(0.5 * m_timeout)));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " float = ", p_float,
                       ", int = ", p_int, ", double = ", p_double);
    };

    function<void(thread::id)> _timeout_callback =
        [this](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    float _float{-340.2};
    int16_t _int{0};
    double _double{3.14};

    typedef tuple<float, int16_t, double> value;

    function<optional<value>()> _provider = [this, &_float, &_int,
                                             &_double]() -> optional<value> {
      _int += 30;
      _float += 2;
      _double += _int;
      value _value{_float, _int, _double};
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _breaker,
                           _provider, _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_005"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

// #######################################

struct async_loop_006 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, no params, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false> async_loop;

    bool _is_timeout{false};
    uint16_t _counter{0};

    function<void()> _worker = [this, &_counter]() -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
      m_cond.notify_one();
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock, [&_is_timeout]() -> bool { return _is_timeout; });

    _async_loop.stop();

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return _is_timeout;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_006"};

  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_007 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, no params, no timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false> async_loop;

    uint16_t _counter{0};

    function<void()> _worker = [this, &_counter]() -> void {
      if (_counter == m_max) {
        concurrent_debug(m_log, "counter reached ", m_max);
        m_cond.notify_one();
        return;
      }
      this_thread::sleep_for(
          chrono::milliseconds(static_cast<uint16_t>(0.5 * m_timeout)));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter);
    };

    function<void(thread::id)> _timeout_callback =
        [this](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_007"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_008 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, one param, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false, int32_t>
        async_loop;

    uint16_t _counter{0};
    bool _is_timeout{false};

    function<void(int32_t)> _worker = [this,
                                       &_counter](int32_t p_value) -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " value = ", p_value);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
      m_cond.notify_one();
    };

    int32_t _value{0};
    function<optional<int32_t>()> _provider = [this,
                                               &_value]() -> optional<int32_t> {
      _value += 30;
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _provider,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    _async_loop.stop();

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    return _is_timeout;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_008"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_009 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, one param, no timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false, int32_t>
        async_loop;

    uint16_t _counter{0};

    function<void(int32_t)> _worker = [this,
                                       &_counter](int32_t p_value) -> void {
      if (_counter == m_max) {
        m_cond.notify_one();
        return;
      }
      this_thread::sleep_for(
          chrono::milliseconds(static_cast<uint16_t>(0.5 * m_timeout)));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " value = ", p_value);
    };

    function<void(thread::id)> _timeout_callback =
        [this](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    int32_t _value{0};
    function<optional<int32_t>()> _provider = [this,
                                               &_value]() -> optional<int32_t> {
      _value += 30;
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _provider,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    _async_loop.stop();

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_009"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_010 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, many params, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false, int32_t, double,
                                     string>
        async_loop;

    uint16_t _counter{0};
    bool _is_timeout{false};

    function<void(int32_t, double, string)> _worker =
        [this, &_counter](int32_t p_int, double p_double,
                          string p_str) -> void {
      if (_counter == static_cast<uint16_t>(m_max / 2)) {
        concurrent_debug(m_log, "worker will sleep enough for timeout");
        this_thread::sleep_for(
            chrono::milliseconds(static_cast<uint16_t>(1.3 * m_timeout)));
        return;
      }
      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " int = ", p_int,
                       " double = ", p_double, ", string = ", p_str);
    };

    function<void(thread::id)> _timeout_callback =
        [this, &_is_timeout](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
      _is_timeout = true;
      m_cond.notify_one();
    };

    int32_t _int{0};
    double _double{3.14};

    typedef tuple<int32_t, double, string> value;

    function<optional<value>()> _provider =
        [this, &_int, &_double]() -> optional<tuple<int32_t, double, string>> {
      _int += 30;
      _double += _int;
      string _str{"int & double = " + to_string(_int) + " " +
                  to_string(_double)};
      value _value{_int, _double, _str};
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _provider,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    _async_loop.stop();

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    concurrent_debug(m_log, "timeout? ", (_is_timeout ? "yes" : "no"));

    return _is_timeout;
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_010"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

struct async_loop_011 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no breaker, many params, no timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;
    using namespace tenacitas;

    typedef concurrent::async_loop_t<logger::cerr::log, false, float, int16_t,
                                     double>
        async_loop;

    uint16_t _counter{0};

    function<void(float, int16_t, double)> _worker =
        [this, &_counter](float p_float, int16_t p_int,
                          double p_double) -> void {
      if (_counter == m_max) {
        concurrent_debug(m_log, "counter reached ", m_max);
        m_cond.notify_one();
        return;
      }
      this_thread::sleep_for(
          chrono::milliseconds(static_cast<uint16_t>(0.5 * m_timeout)));

      ++_counter;
      concurrent_debug(m_log, "counter = ", _counter, " float = ", p_float,
                       ", int = ", p_int, ", double = ", p_double);
    };

    function<void(thread::id)> _timeout_callback =
        [this](thread::id p_id) -> void {
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    float _float{-340.2};
    int16_t _int{0};
    double _double{3.14};

    typedef tuple<float, int16_t, double> value;

    function<optional<value>()> _provider = [this, &_float, &_int,
                                             &_double]() -> optional<value> {
      _int += 30;
      _float += 2;
      _double += _int;
      value _value{_float, _int, _double};
      concurrent_debug(m_log, "providing value ", _value);
      return {_value};
    };

    async_loop _async_loop(chrono::milliseconds(m_timeout), _worker, _provider,
                           _timeout_callback);

    _async_loop.start();

    unique_lock<mutex> _lock(m_mutex);
    m_cond.wait(_lock);

    concurrent_debug(m_log, "notification that the loop stopped arrived");

    _async_loop.stop();

    return (_counter == m_max);
  }

private:
  tenacitas::logger::cerr::log m_log{"async_loop_011"};
  std::condition_variable m_cond;
  std::mutex m_mutex;

  static constexpr uint16_t m_max{20};
  static constexpr uint16_t m_timeout{1000};
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();

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
  run_test(_tester, async_loop_009);
  run_test(_tester, async_loop_010);
  run_test(_tester, async_loop_011);
}
