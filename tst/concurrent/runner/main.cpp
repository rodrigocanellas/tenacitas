#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>

#include <concurrent/runner.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct runner_000 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 int32_t, int16_t>
        runner;

    auto _worker = [](int16_t p_int) -> int32_t {
      this_thread::sleep_for(chrono::milliseconds(2000));
      return p_int + 1;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "not timeout when it should");
      return false;
    }

    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: one; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_000"};
};

struct runner_001 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 int32_t, int16_t>
        runner;

    auto _worker = [](int16_t p_int) -> int32_t {
      this_thread::sleep_for(chrono::milliseconds(500));
      return p_int + 4;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    optional<int32_t> _maybe = _runner(73);

    if (!_maybe) {
      concurrent_error(m_log, "failed to return, when it should not");
      return false;
    }

    concurrent_debug(m_log, "run was ok");

    int32_t _res = _maybe.value();
    if (_res != 77) {
      concurrent_error(m_log, "returned ", _res, ", but it should be ", 77);
      return false;
    }

    concurrent_debug(m_log, "value is ", _res, ", as it should");

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: one; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_001"};
};

struct runner_002 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 double, int16_t, float>
        runner;

    auto _worker = [](int16_t p_int, float p_float) -> double {
      this_thread::sleep_for(chrono::milliseconds(2000));
      return static_cast<double>(p_int) + p_float;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73, 1.5);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "no timeout when it should");
      return false;
    }

    concurrent_debug(m_log, "timeout, as it should");
    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: many; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_002"};
};

struct runner_003 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 double, int16_t, float>
        runner;

    auto _worker = [](int16_t p_int, float p_float) -> double {
      this_thread::sleep_for(chrono::milliseconds(500));
      return static_cast<double>(p_int) + p_float;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    optional<double> _maybe = _runner(73, 1.5);

    if (!_maybe) {
      concurrent_error(m_log, "failed to return, when it should not");
      return false;
    }

    concurrent_debug(m_log, "run was ok");

    double _res = _maybe.value();
    if (_res != (static_cast<double>(73) + 1.5)) {
      concurrent_error(m_log, "returned ", _res, ", but it should be ", 77);
      return false;
    }

    concurrent_debug(m_log, "value is ", _res, ", as it should");

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: many; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_003"};
};

struct runner_004 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 double>
        runner;

    auto _worker = []() -> double {
      this_thread::sleep_for(chrono::milliseconds(2000));
      return 3.14;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner();

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "no timeout when it should");
      return false;
    }

    concurrent_debug(m_log, "timeout, as it should");
    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_004"};
};

struct runner_005 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 double>
        runner;

    auto _worker = []() -> double {
      this_thread::sleep_for(chrono::milliseconds(500));
      return 3.14;
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    optional<double> _maybe = _runner();

    if (!_maybe) {
      concurrent_error(m_log, "failed to return, when it should not");
      return false;
    }

    concurrent_debug(m_log, "run was ok");

    double _res = _maybe.value();
    if (_res != 3.14) {
      concurrent_error(m_log, "returned ", _res, ", but it should be ", 3.14);
      return false;
    }

    concurrent_debug(m_log, "value is ", _res, ", as it should");

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: yes; parameter: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_005"};
};

struct runner_006 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void, int16_t>
        runner;

    auto _worker = [this](int16_t p_int) -> void {
      this_thread::sleep_for(chrono::milliseconds(2000));
      concurrent_debug(m_log, "parameter is ", p_int);
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "not timeout when it should");
      return false;
    }

    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: one; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_006"};
};

struct runner_007 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void, int16_t>
        runner;

    auto _worker = [this](int16_t p_int) -> void {
      this_thread::sleep_for(chrono::milliseconds(500));
      concurrent_debug(m_log, "parameter is ", p_int);
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: one; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_007"};
};

struct runner_008 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void, int16_t, float>
        runner;

    auto _worker = [this](int16_t p_int, float p_float) -> void {
      this_thread::sleep_for(chrono::milliseconds(2000));
      concurrent_debug(m_log, "int = ", p_int, ", float = ", p_float);
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73, 1.5);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "no timeout when it should");
      return false;
    }

    concurrent_debug(m_log, "timeout, as it should");
    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: many; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_008"};
};

struct runner_009 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void, int16_t, float>
        runner;

    auto _worker = [this](int16_t p_int, float p_float) -> void {
      this_thread::sleep_for(chrono::milliseconds(500));
      concurrent_debug(m_log, "int = ", p_int, ", float = ", p_float);
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner(73, 1.5);

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: many; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_009"};
};

struct runner_010 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void>
        runner;

    auto _worker = [this]() -> void {
      this_thread::sleep_for(chrono::milliseconds(2000));
      concurrent_debug(m_log, "worker in action");
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner();

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (!_is_timeout) {
      concurrent_error(m_log, "no timeout when it should");
      return false;
    }

    concurrent_debug(m_log, "timeout, as it should");
    return _is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: no; timeout: yes";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_010"};
};

struct runner_011 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::milliseconds,
                                 void>
        runner;

    auto _worker = [this]() -> void {
      this_thread::sleep_for(chrono::milliseconds(500));
      concurrent_debug(m_log, "worker in action");
    };

    bool _is_timeout{false};

    auto _timeout_callback = [this, &_is_timeout](thread::id p_id) -> void {
      _is_timeout = true;
      concurrent_warn(m_log, "timeout for ", p_id);
    };

    runner _runner(std::chrono::milliseconds(1000), _worker, _timeout_callback);

    _runner();

    this_thread::sleep_for(chrono::milliseconds(3000));

    if (_is_timeout) {
      concurrent_error(m_log, "timeout when it should not");
      return false;
    }

    concurrent_debug(m_log, "no timeout, as it should");
    return !_is_timeout;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "return: no; parameter: no; timeout: no";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_011"};
};

struct runner_012 {

  bool operator()() {
    using namespace tenacitas;
    using namespace std;

    typedef concurrent::runner_t<logger::cerr::log, chrono::milliseconds, void,
                                 int16_t, int32_t, float>
        runner;

    auto _worker = [this](int16_t p_i1, int32_t p_i2, float p_float) -> void {
      concurrent_debug(m_log, "i1 = ", p_i1, ", i2 = ", p_i2,
                       ", float = ", p_float);
    };

    runner _runner(std::chrono::milliseconds(300), _worker);

    int16_t _i1 = 0;
    int32_t _i2 = 100;
    float _f = 0.0;
    while (_i1 < 30) {
      _i2 += 10;
      _f += _i2 * -1.5;

      _runner(_i1, _i2, _f);

      ++_i1;

      this_thread::sleep_for(chrono::milliseconds(500));
    }

    return true;
  }

  static std::string desc() {
    std::stringstream _stream;
    _stream << "Runner with no return, 3 parameters: i1 (int16), i2 (int32), f "
               "(float).\n"
            << "A loop in i1 from 0 to 29, calling runner with i1, i2 + 10, i2 "
               "* -1.5";
    return _stream.str();
  }

private:
  tenacitas::logger::cerr::log m_log{"runner_012"};
};

int main(int argc, char **argv) {

  tenacitas::logger::cerr::log::set_debug();
  tenacitas::tester::test _test(argc, argv);
  run_test(_test, runner_000);
  run_test(_test, runner_001);
  run_test(_test, runner_002);
  run_test(_test, runner_003);
  run_test(_test, runner_004);
  run_test(_test, runner_005);
  run_test(_test, runner_006);
  run_test(_test, runner_007);
  run_test(_test, runner_008);
  run_test(_test, runner_009);
  run_test(_test, runner_010);
  run_test(_test, runner_011);
  run_test(_test, runner_012);

  return 0;
}
