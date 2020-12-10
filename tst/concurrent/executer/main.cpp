#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>

#include <concurrent/executer.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

typedef logger::cerr::log log;

struct test900 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, double, int, float>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, double,
                                   int, float>
        executer;

    auto function = [this](int p_i, float p_f) -> double {
      concurrent_debug(m_log, "i = ", p_i, ", f = ", p_f);
      return p_i * p_f;
    };

    executer _executer(function);

    int _i = 2;
    float _f = 3.5;

    std::optional<double> _maybe = _executer(_i, _f);

    if (!_maybe) {
      concurrent_error(m_log, "nothing returned, but it should have");
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * _f)) {
      concurrent_error(m_log, "expected ", _i * _f, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test900"};
};

struct test901 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, double, int>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, double,
                                   int>
        executer;

    auto function = [this](int p_i) -> double {
      concurrent_debug(m_log, "i = ", p_i);
      return p_i * 2.5;
    };

    executer _executer(function);

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (!_maybe) {
      concurrent_error(m_log, "nothing returned, but it should have");
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * 2.5)) {
      concurrent_error(m_log, "expected ", _i * 2.5, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test901"};
};

struct test902 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, double, void>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, double,
                                   void>
        executer;

    auto function = []() -> double { return 3.14; };

    executer _executer(function);

    std::optional<double> _maybe = _executer();

    if (!_maybe) {
      concurrent_error(m_log, "nothing returned, but it should have");
    }

    double _d = std::move(*_maybe);

    if (_d != (3.14)) {
      concurrent_error(m_log, "expected ", 3.14, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test902"};
};

struct test903 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, void, int, float>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, void,
                                   int, float>
        executer;

    double _d{0};

    auto function = [this, &_d](int p_i, float p_f) -> void {
      concurrent_debug(m_log, "i = ", p_i, ", f = ", p_f);
      _d = p_i * p_f;
    };

    executer _executer(function);

    int _i = 2;
    float _f = 3.5;

    _executer(_i, _f);

    if (_d != (_i * _f)) {
      concurrent_error(m_log, "expected ", _i * _f, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test903"};
};

struct test904 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, void, int>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, void,
                                   int>
        executer;

    double _d{0};

    auto function = [this, &_d](int p_i) -> void {
      concurrent_debug(m_log, "i = ", p_i);
      _d = p_i * 2.5;
    };

    executer _executer(function);

    int _i = 2;

    _executer(_i);

    if (_d != (_i * 2.5)) {
      concurrent_error(m_log, "expected ", _i * 2.5, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test904"};
};

struct test905 {

  static std::string desc() {
    return "executer as <concurrent::timeout_control::no, void, void>";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, concurrent::timeout_control::no, void,
                                   void>
        executer;

    double _d{0};

    auto function = [&_d]() -> void { _d = 2.5; };

    executer _executer(function);

    _executer();

    if (_d != 2.5) {
      concurrent_error(m_log, "expected ", 2.5, ", but got ", _d);
      return false;
    }

    concurrent_debug(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
  }

private:
  log m_log{"test905"};
};

struct test00 {

  static std::string desc() {
    return "no timeout control; return; many parameters";
  }

  bool operator()() { return true; }

private:
  log m_log{"test00"};
};

struct test01 {
  static std::string desc() {
    return "no timeout control; return; one parameter";
  }

  bool operator()() { return true; }

private:
  log m_log{"test01"};
};

struct test02 {
  static std::string desc() {
    return "no timeout control; return; no parameter";
  }

  bool operator()() { return true; }

private:
  log m_log{"test02"};
};

struct test03 {
  static std::string desc() {
    return "no timeout control; no return; many parameters";
  }

  bool operator()() { return true; }

private:
  log m_log{"test03"};
};

struct test04 {
  static std::string desc() {
    return "no timeout control; no return; one parameter";
  }

  bool operator()() { return true; }

private:
  log m_log{"test04"};
};

struct test05 {
  static std::string desc() {
    return "no timeout control; no return; no parameter";
  }

  bool operator()() { return true; }

private:
  log m_log{"test05"};
};

struct test06 {
  static std::string desc() {
    return "timeout control; return; many parameterss; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test06"};
};

struct test07 {
  static std::string desc() {
    return "timeout control; return; many parameterss; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test07"};
};

struct test08 {
  static std::string desc() {
    return "timeout control; return; one parameter; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test08"};
};

struct test09 {
  static std::string desc() {
    return "timeout control; return; one parameter; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test09"};
};

struct test10 {
  static std::string desc() {
    return "timeout control; return; no parameter; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test10"};
};

struct test11 {
  static std::string desc() {
    return "timeout control; return; no parameter; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test11"};
};

struct test12 {
  static std::string desc() {
    return "timeout control; no return; many parameterss; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test12"};
};

struct test13 {
  static std::string desc() {
    return "timeout control; no return; many parameterss; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test13"};
};

struct test14 {
  static std::string desc() {
    return "timeout control; no return; one parameter; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test14"};
};

struct test15 {
  static std::string desc() {
    return "timeout control; no return; one parameter; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test15"};
};

struct test16 {
  static std::string desc() {
    return "timeout control; no return; no parameter; with timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test16"};
};

struct test17 {
  static std::string desc() {
    return "timeout control; no return; no parameter; without timeout";
  }

  bool operator()() { return true; }

private:
  log m_log{"test17"};
};

int main(int argc, char **argv) {

  tenacitas::logger::cerr::log::set_debug();
  tenacitas::tester::test _test(argc, argv);
  run_test(_test, test900);
  run_test(_test, test901);
  run_test(_test, test902);
  run_test(_test, test903);
  run_test(_test, test904);
  run_test(_test, test905);

  return 0;
}
