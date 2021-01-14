#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

using namespace std::chrono_literals;

struct timeout_callback {
  inline void operator()(std::thread::id p_id) {
    WAR(m_log, "timeout for ", p_id);
  }

private:
  logger::cerr<> m_log{"timeout_callback"};
};

timeout_callback _timeout_callback;

struct test000 {

  static std::string desc() {
    return "executer as executer_t<logger, double, int, float>, no timeout";
  }

  bool operator()() {

    typedef concurrent::executer_t<logger::cerr<>, double, int, float> executer;

    m_log.set_debug_level();

    auto function = [this](int p_i, float p_f) -> double {
      DEB(m_log, "i = ", p_i, ", f = ", p_f);
      return p_i * p_f;
    };

    DEB(m_log, "about to create");
    executer _executer(function, 1s, _timeout_callback);
    DEB(m_log, "created");

    _executer.start();

    int _i = 2;
    float _f = 3.5;

    DEB(m_log, "about to call");
    std::optional<double> _maybe = _executer(_i, _f);

    if (!_maybe) {
      ERR(m_log, "nothing returned, but it should have");
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * _f)) {
      ERR(m_log, "expected ", _i * _f, ", but got ", _d);
      return false;
    }

    DEB(m_log, "got ", _d, ", as expected");

    std::this_thread::sleep_for(1s);
    return true;
  }

private:
  logger::cerr<> m_log{"test000"};
};

struct test100 {

  static std::string desc() {
    return "executer as executer_t<logger, double, int> with timeout";
  }

  bool operator()() {

    m_log.set_debug_level();

    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

    std::chrono::seconds _timeout(1);

    auto _worker = [this](int p_i) -> double {
      std::this_thread::sleep_for(2s);
      DEB(m_log, "i = ", p_i);
      return p_i * 2.5;
    };

    executer _executer(_worker, _timeout, _timeout_callback);

    _executer.start();

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      ERR(m_log, "returned, but it should have not");
      return false;
    }

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(5s);
    DEB(m_log, "waking");
    return true;
  }

private:
  logger::cerr<> m_log{"test100"};
};

struct test101 {

  static std::string desc() {
    return "executer as executer_t<logger, double, int> with timeout";
  }

  bool operator()() {

    m_log.set_debug_level();

    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

    std::chrono::seconds _timeout(1);

    auto function = [this](int p_i) -> double {
      std::this_thread::sleep_for(2s);
      DEB(m_log, "i = ", p_i);
      return p_i * 2.5;
    };

    executer _executer(function, _timeout, timeout_callback());

    _executer.start();

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      ERR(m_log, "returned, but it should have not");
      return false;
    }

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(5s);
    DEB(m_log, "waking");
    return true;
  }

private:
  logger::cerr<> m_log{"test101"};
};

struct test200 {

  static std::string desc() {
    return "executer as executer_t<logger, double, int>, with timeout, and "
           "function returned before the next call, which did not timeout";
  }

  bool operator()() {

    m_log.set_debug_level();

    const std::chrono::seconds _timeout{1};
    std::chrono::milliseconds _sleep{2000};

    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

    auto _function = [this, &_sleep](int p_i) -> double {
      DEB(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(), "ms");
      std::this_thread::sleep_for(_sleep);
      return p_i * 2.5;
    };

    executer _executer(_function, _timeout, timeout_callback());

    _executer.start();

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      ERR(m_log, "a timeout should have occurred");
      return false;
    }

    DEB(m_log,
        "timeout in the first call, sleeping to allow the function to end");

    std::this_thread::sleep_for(1s);

    _sleep = 500ms;

    DEB(m_log, "running again, and no timeout should occurr");

    _maybe = _executer(_i);

    if (!_maybe) {
      ERR(m_log, "it was not possible to execute, but it should have been");
      return false;
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * 2.5)) {
      ERR(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
      return false;
    }
    DEB(m_log, "value is ", _d, ", as it should");

    std::this_thread::sleep_for(3s);

    return true;
  }

private:
  logger::cerr<> m_log{"test200"};
};

struct test300 {

  static std::string desc() {
    return "executer as executer_t<logger, double, int>, with timeout, and "
           "function did not returned before the next call, which did not "
           "timeout";
  }

  bool operator()() {

    m_log.set_debug_level();

    const std::chrono::seconds _timeout{1};
    std::chrono::milliseconds _sleep{4000};

    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

    auto _function = [this, &_sleep](int p_i) -> double {
      DEB(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(), "ms");
      std::this_thread::sleep_for(_sleep);
      return p_i * 2.5;
    };

    executer _executer(_function, _timeout, timeout_callback());

    _executer.start();

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      ERR(m_log, "a timeout should have occurred");
      return false;
    }

    DEB(m_log, "timeout in the first call, sleeping not enough to "
               "allow the function to end");

    std::this_thread::sleep_for(500ms);

    _sleep = 500ms;

    DEB(m_log, "running again, and no timeout should occurr because sleep is ",
        _sleep.count(), "millisecs");

    _maybe = _executer(_i);

    if (!_maybe) {
      ERR(m_log, "it was not possible to execute, but it should have been");
      return false;
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * 2.5)) {
      ERR(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
      return false;
    }
    DEB(m_log, "value is ", _d, ", as it should");

    std::this_thread::sleep_for(30s);

    return true;
  }

private:
  logger::cerr<> m_log{"test300"};
};

int main(int argc, char **argv) {

  logger::set_debug_level();
  tester::test _test(argc, argv);
  run_test(_test, test000);
  run_test(_test, test100);
  run_test(_test, test101);
  run_test(_test, test200);
  run_test(_test, test300);

  return 0;
}
