#include <cstdint>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

#include <concurrent/executer.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

/// \example

/// \file This file contains tests for the \p executer_t class.
///
/// The tests were base on this state machine below, where the number in
/// parentheses is the number o the test
/*
@startuml

hide empty description

[*] --> waiting_for_work : create

waiting_for_work --> waiting_for_work : start (002)

waiting_for_work --> working : work (004)
                                                                                                              working --> working : start (003)
                                                                                                                                      waiting_for_work --> stopped : stop (000)
                                                                                                                                                                       stopped --> stopped : stop (001)
                                                                                                                                                                                               working --> waiting_for_work : timeout (005)
                                                                                                                                                                                                                                working --> stopped : stop (006)
                                                                                                                                                                                                                                                        working --> waiting_for_work : done (007)
                                                                                                                                                                                                                                                                                         waiting_for_work --> waiting_for_work : return (008)
                                                                                                                                                                                                                                                                                                                                   @enduml
*/

struct executer_000 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker =
        [this](int16_t p_int, double p_double) -> std::optional<bool> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "waiting_for_work --> stopped : stop. \nStopping an executer (with "
           "parameters and return) that was not called.";
  }

private:
  logger::cerr::log m_log{"executer_000"};
};

struct executer_001 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker =
        [this](int16_t p_int, double p_double) -> std::optional<bool> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    if (!_executer.is_stopped()) {
      concurrent_error(m_log, "it should be stopped, but it is not");
      return false;
    }

    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "stopped --> stopped : stop (001). \nStopping an executer (with "
           "parameters and return) that was not called, and it was already "
           "stopped";
  }

private:
  logger::cerr::log m_log{"executer_001"};
};

struct executer_002 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool,

                                   int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker =
        [this](int16_t p_int, double p_double) -> std::optional<bool> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    if (!_executer.is_started()) {
      concurrent_error(m_log, "it should be running, but it is not");
      return false;
    }

    _executer.start();

    return _executer.is_started();
  }

  static std::string desc() {
    return "waiting_for_work --> waiting_for_work : start (002). \nStarting an "
           "executer (with parameters and return) that is started";
  }

private:
  logger::cerr::log m_log{"executer_002"};
};

struct executer_003 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker =
        [this](int16_t p_int, double p_double) -> std::optional<bool> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "starting again");
    _executer.start();

    _thread.join();

    return _executer.is_started();
  }

  static std::string desc() {
    return "working --> working : start (003).\nStarting an executer (with "
           "parameters and return) that is started, and after call to "
           "'operator()()";
  }

private:
  logger::cerr::log m_log{"executer_003"};
};

struct executer_004 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, int16_t, double>
        executer;

    const uint16_t _timeout = 6000;

    typename executer::worker _worker =
        [this](int16_t p_int, double p_double) -> std::optional<double> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {p_int * p_double};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    _thread.join();

    if (!_ok) {
      return false;
    }

    concurrent_debug(m_log, "value = ", _d);

    return (_d == (-2 * 3.14));
  }

  static std::string desc() {
    return "waiting_for_work --> working : work (004).\nCalling an executer "
           "(with parameters and return)";
  }

private:
  logger::cerr::log m_log{"executer_004"};
};

struct executer_005 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker =
        [this, _timeout](int16_t p_int,
                         double p_double) -> std::optional<double> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 1.1)));
      concurrent_debug(m_log, "waking");

      return {p_int * p_double};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? false : true);
  }

  static std::string desc() {
    return "working --> waiting_for_work : timeout (005).\nCalling an executer "
           "(with parameters and return), but a timeout occurred";
  }

private:
  logger::cerr::log m_log{"executer_005"};
};

struct executer_006 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker =
        [this, _timeout](int16_t p_int,
                         double p_double) -> std::optional<double> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");

      return {p_int * p_double};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    _executer.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? false : true);
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with "
           "parameters and return), and stopped while working";
  }

private:
  logger::cerr::log m_log{"executer_006"};
};

struct executer_007 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker =
        [this, _timeout](int16_t p_int,
                         double p_double) -> std::optional<double> {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");

      return {p_int * p_double};
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "expected ", -2 * 3.14, ", got ", _d);
        _ok = (_d == (-2 * 3.14));
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? true : false);
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with "
           "parameters and return), and the work is done";
  }

private:
  logger::cerr::log m_log{"executer_007"};
};

// #############################################################################

struct executer_008 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> std::optional<bool> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "waiting_for_work --> stopped : stop. \nStopping an executer (with "
           "no parameters and with return) that was not called.";
  }

private:
  logger::cerr::log m_log{"executer_008"};
};

struct executer_009 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> std::optional<bool> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    if (!_executer.is_stopped()) {
      concurrent_error(m_log, "it should be stopped, but it is not");
      return false;
    }

    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "stopped --> stopped : stop (001). \nStopping an executer (with "
           "no parameters and with return) that was not called, and it was "
           "already stopped";
  }

private:
  logger::cerr::log m_log{"executer_009"};
};

struct executer_010 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> std::optional<bool> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    if (!_executer.is_started()) {
      concurrent_error(m_log, "it should be running, but it is not");
      return false;
    }

    _executer.start();

    return _executer.is_started();
  }

  static std::string desc() {
    return "waiting_for_work --> waiting_for_work : start (002). \nStarting an "
           "executer (with no parameters and with return) that is started";
  }

private:
  logger::cerr::log m_log{"executer_010"};
};

struct executer_011 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   bool, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> std::optional<bool> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {true};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "starting again");
    _executer.start();

    _thread.join();

    return _executer.is_started();
  }

  static std::string desc() {
    return "working --> working : start (003).\nStarting an executer (with no "
           "parameters and with return) that is started, and after call to "
           "'operator()()";
  }

private:
  logger::cerr::log m_log{"executer_011"};
};

struct executer_012 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, void>
        executer;

    const uint16_t _timeout = 6000;

    typename executer::worker _worker = [this]() -> std::optional<double> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return {3.14};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    _thread.join();

    if (!_ok) {
      return false;
    }

    concurrent_debug(m_log, "value = ", _d);

    return (_d == 3.14);
  }

  static std::string desc() {
    return "waiting_for_work --> working : work (004).\nCalling an executer "
           "(with no parameters and with return)";
  }

private:
  logger::cerr::log m_log{"executer_012"};
};

struct executer_013 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this,
                                         _timeout]() -> std::optional<double> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 1.1)));
      concurrent_debug(m_log, "waking");

      return {3.14};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? false : true);
  }

  static std::string desc() {
    return "working --> waiting_for_work : timeout (005).\nCalling an executer "
           "(with no parameters and with return), but a timeout occurred";
  }

private:
  logger::cerr::log m_log{"executer_013"};
};

struct executer_014 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this,
                                         _timeout]() -> std::optional<double> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");

      return {3.14};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "calculated value = ", _d);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    _executer.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? false : true);
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with no "
           "parameters and with return), and stopped while working";
  }

private:
  logger::cerr::log m_log{"executer_014"};
};

struct executer_015 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   double, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this,
                                         _timeout]() -> std::optional<double> {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");

      return {3.14};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    double _d = 0.0;
    bool _ok = true;

    std::thread _thread([this, &_executer, &_d, &_ok]() -> void {
      std::optional<double> _ret = _executer();
      if (_ret) {
        _d = _ret.value();
        concurrent_debug(m_log, "expected ", 3.14, ", got ", _d);
        _ok = (_d == 3.14);
      } else {
        concurrent_debug(m_log, "error in execution");
        _ok = false;
      }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return (_ok ? true : false);
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with no "
           "parameters and with return), and the work is done";
  }

private:
  logger::cerr::log m_log{"executer_015"};
};

// #############################################################################
struct executer_016 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "waiting_for_work --> stopped : stop. \nStopping an executer (with "
           "parameters and no return) that was not called.";
  }

private:
  logger::cerr::log m_log{"executer_016"};
};

struct executer_017 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    if (!_executer.is_stopped()) {
      concurrent_error(m_log, "it should be stopped, but it is not");
      return false;
    }

    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "stopped --> stopped : stop (001). \nStopping an executer (with "
           "parameters and no return) that was not called, and it was already "
           "stopped";
  }

private:
  logger::cerr::log m_log{"executer_017"};
};

struct executer_018 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    if (!_executer.is_started()) {
      concurrent_error(m_log, "it should be running, but it is not");
      return false;
    }

    _executer.start();

    return _executer.is_started();
  }

  static std::string desc() {
    return "waiting_for_work --> waiting_for_work : start (002). \nStarting an "
           "executer (with parameters and no return) that is started";
  }

private:
  logger::cerr::log m_log{"executer_018"};
};

struct executer_019 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "starting again");
    _executer.start();

    _thread.join();

    return _executer.is_started();
  }

  static std::string desc() {
    return "working --> working : start (003).\nStarting an executer (with "
           "parameters and no return) that is started, and after call to "
           "'operator()()";
  }

private:
  logger::cerr::log m_log{"executer_019"};
};

struct executer_020 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const uint16_t _timeout = 6000;

    bool _ok = true;
    typename executer::worker _worker = [this, &_ok](int16_t p_int,
                                                     double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      if ((p_int != -2) && (p_double != 3.14)) {
        concurrent_error(m_log, "values should be -2 and 3,14");
        _ok = false;
      }
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
      _ok = true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    _thread.join();

    return _ok;
  }

  static std::string desc() {
    return "waiting_for_work --> working : work (004).\nCalling an executer "
           "(with parameters and no return)";
  }

private:
  logger::cerr::log m_log{"executer_020"};
};

struct executer_021 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000);

    typename executer::worker _worker =
        [this, _timeout](int16_t p_int, double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 1.1)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "working --> waiting_for_work : timeout (005).\nCalling an executer "
           "(with parameters and no return), but a timeout occurred.\nTo be "
           "certain that the there was a timeout, one must look at the log for "
           "'worker did not finish in time' message.\nThis is necessary "
           "because the work function does not return";
  }

private:
  logger::cerr::log m_log{"executer_005"};
};

struct executer_022 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    double _d = 0.0;
    typename executer::worker _worker =
        [this, _timeout, &_d](int16_t p_int, double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      _d = p_int * p_double;
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    _executer.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    concurrent_debug(m_log, "_d = ", _d);

    return (_d == 0.0);
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with "
           "parameters and no return), and stopped while working";
  }

private:
  logger::cerr::log m_log{"executer_022"};
};

struct executer_023 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, int16_t, double>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    double _d = 0.0;
    typename executer::worker _worker =
        [this, _timeout, &_d](int16_t p_int, double p_double) -> void {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      _d = -2 * 3.14;
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-2, 3.14}};
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout), _provider);

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    concurrent_debug(m_log, "_d = ", _d);

    return (_d == (-2 * 3.14));
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with "
           "parameters and no return), and the work is done";
  }

private:
  logger::cerr::log m_log{"executer_023"};
};

// #############################################################################

struct executer_024 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "waiting_for_work --> stopped : stop. \nStopping an executer (with "
           "no parameters and no return) that was not called.";
  }

private:
  logger::cerr::log m_log{"executer_024"};
};

struct executer_025 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    if (!_executer.is_stopped()) {
      concurrent_error(m_log, "it should be stopped, but it is not");
      return false;
    }

    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "stopped --> stopped : stop (001). \nStopping an executer (with no "
           "parameters and no return) that was not called, and it was already "
           "stopped";
  }

private:
  logger::cerr::log m_log{"executer_025"};
};

struct executer_026 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    if (!_executer.is_started()) {
      concurrent_error(m_log, "it should be running, but it is not");
      return false;
    }

    _executer.start();

    return _executer.is_started();
  }

  static std::string desc() {
    return "waiting_for_work --> waiting_for_work : start (002). \nStarting an "
           "executer (with no parameters and no return) that is started";
  }

private:
  logger::cerr::log m_log{"executer_026"};
};

struct executer_027 {
  bool operator()() {

    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "starting again");
    _executer.start();

    _thread.join();

    return _executer.is_started();
  }

  static std::string desc() {
    return "working --> working : start (003).\nStarting an executer (with no "
           "parameters and no return) that is started, and after call to "
           "'operator()()";
  }

private:
  logger::cerr::log m_log{"executer_027"};
};

struct executer_028 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const uint16_t _timeout = 6000;

    typename executer::worker _worker = [this]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "waiting_for_work --> working : work (004).\nCalling an executer "
           "(with no parameters and no return).\nThe only way to find out "
           "that the work function really worked, and on time, is looking in "
           "the log, as the work return no data";
  }

private:
  logger::cerr::log m_log{"executer_028"};
};

struct executer_029 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this, _timeout]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 1.1)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "working --> waiting_for_work : timeout (005).\nCalling an executer "
           "(with no parameters and no return), but a timeout occurred.\nThe "
           "only way to find out that the work function really timedout is "
           "looking in the log, as the work return no data";
  }

private:
  logger::cerr::log m_log{"executer_029"};
};

struct executer_030 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this, _timeout]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    _executer.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with no "
           "parameters and no return), and stopped while working.\nThe only "
           "way to find out that the work function was really stopped is "
           "looking in the log, as the work return no data";
  }

private:
  logger::cerr::log m_log{"executer_030"};
};

struct executer_031 {
  bool operator()() {
    typedef concurrent::executer_t<logger::cerr::log, std::chrono::milliseconds,
                                   void, void>
        executer;

    const std::chrono::milliseconds _timeout(3000); // millises

    typename executer::worker _worker = [this, _timeout]() -> void {
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(std::chrono::milliseconds(
          static_cast<uint16_t>(_timeout.count() * 0.7)));
      concurrent_debug(m_log, "waking");
    };

    executer _executer(_worker, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(
        static_cast<uint16_t>(_timeout.count() * 1.3)));

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "working --> stopped : stop (006).\nCalling an executer (with no "
           "parameters and no return), and the work is done.\nThe only "
           "way to find out that the work function really worked is "
           "looking in the log, as the work return no data";
  }

private:
  logger::cerr::log m_log{"executer_031"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, executer_000);
  run_test(_test, executer_001);
  run_test(_test, executer_002);
  run_test(_test, executer_003);
  run_test(_test, executer_004);
  run_test(_test, executer_005);
  run_test(_test, executer_006);
  run_test(_test, executer_007);
  // ###########################
  run_test(_test, executer_008);
  run_test(_test, executer_009);
  run_test(_test, executer_010);
  run_test(_test, executer_011);
  run_test(_test, executer_012);
  run_test(_test, executer_013);
  run_test(_test, executer_014);
  run_test(_test, executer_015);
  // ###########################
  run_test(_test, executer_016);
  run_test(_test, executer_017);
  run_test(_test, executer_018);
  run_test(_test, executer_019);
  run_test(_test, executer_020);
  run_test(_test, executer_021);
  run_test(_test, executer_022);
  run_test(_test, executer_023);
  // ###########################
  run_test(_test, executer_024);
  run_test(_test, executer_025);
  run_test(_test, executer_026);
  run_test(_test, executer_027);
  run_test(_test, executer_028);
  run_test(_test, executer_029);
  run_test(_test, executer_030);
  run_test(_test, executer_031);
}
