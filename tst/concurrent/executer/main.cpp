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

void timeout_callback(std::thread::id p_id) {
  log _log{"timeout_callback "};
  concurrent_warn(_log, "timeout for ", p_id);
}

struct test060 {

  static std::string desc() {
    return "executer as < double, int, "
           "float>, no timeout";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, double, int, float> executer;

    auto function = [this](int p_i, float p_f) -> double {
      concurrent_debug(m_log, "i = ", p_i, ", f = ", p_f);
      return p_i * p_f;
    };

    concurrent_debug(m_log, "about to create");
    executer _executer(function, std::chrono::seconds(1), timeout_callback);
    concurrent_debug(m_log, "created");

    int _i = 2;
    float _f = 3.5;

    concurrent_debug(m_log, "about to call");
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
  log m_log{"test060"};
};

struct test070 {

  static std::string desc() {
    return "executer as < double, int>, no "
           "timeout";
  }

  bool operator()() {

    typedef concurrent::executer_t<log, double, int> executer;

    auto function = [this](int p_i) -> double {
      concurrent_debug(m_log, "i = ");
      return p_i * 2.5;
    };

    executer _executer(function, std::chrono::seconds(1), timeout_callback);

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
  log m_log{"test070"};
};

struct test071 {

  static std::string desc() {
    return "executer as < double, int>, with "
           "timeout, and function returned before the next call, which did not "
           "timeout";
  }

  bool operator()() {

    const std::chrono::seconds _timeout{1};
    std::chrono::milliseconds _sleep{2000};

    typedef concurrent::executer_t<log, double, int> executer;

    auto _function = [this, &_sleep](int p_i) -> double {
      concurrent_debug(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(),
                       "ms");
      std::this_thread::sleep_for(_sleep);
      return p_i * 2.5;
    };

    executer _executer(_function, _timeout, timeout_callback);

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      concurrent_error(m_log, "a timeout should have occurred");
      return false;
    }

    concurrent_debug(
        m_log,
        "timeout in the first call, sleeping to allow the function to end");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    _sleep = std::chrono::milliseconds(500);

    concurrent_debug(m_log, "running again, and no timeout should occurr");

    _maybe = _executer(_i);

    if (!_maybe) {
      concurrent_error(
          m_log, "it was not possible to execute, but it should have been");
      return false;
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * 2.5)) {
      concurrent_error(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
      return false;
    }
    concurrent_debug(m_log, "value is ", _d, ", as it should");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return true;
  }

private:
  log m_log{"test071"};
};

struct test072 {

  static std::string desc() {
    return "executer as < double, int>, with "
           "timeout, and function did not returned before the next call, which "
           "did not timeout";
  }

  bool operator()() {

    const std::chrono::seconds _timeout{1};
    std::chrono::milliseconds _sleep{4000};

    typedef concurrent::executer_t<log, double, int> executer;

    auto _function = [this, &_sleep](int p_i) -> double {
      concurrent_debug(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(),
                       "ms");
      std::this_thread::sleep_for(_sleep);
      return p_i * 2.5;
    };

    executer _executer(_function, _timeout, timeout_callback);

    int _i = 2;

    std::optional<double> _maybe = _executer(_i);

    if (_maybe) {
      concurrent_error(m_log, "a timeout should have occurred");
      return false;
    }

    concurrent_debug(m_log, "timeout in the first call, sleeping not enough to "
                            "allow the function to end");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    _sleep = std::chrono::milliseconds(500);

    concurrent_debug(m_log, "running again, and no timeout should occurr");

    _maybe = _executer(_i);

    if (!_maybe) {
      concurrent_error(
          m_log, "it was not possible to execute, but it should have been");
      return false;
    }

    double _d = std::move(*_maybe);

    if (_d != (_i * 2.5)) {
      concurrent_error(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
      return false;
    }
    concurrent_debug(m_log, "value is ", _d, ", as it should");

    std::this_thread::sleep_for(std::chrono::seconds(7));

    return true;
  }

private:
  log m_log{"test072"};
};

int main(int argc, char **argv) {

  tenacitas::logger::cerr::log::set_debug();
  tenacitas::tester::test _test(argc, argv);
  run_test(_test, test060);
  run_test(_test, test070);
  run_test(_test, test071);
  run_test(_test, test072);

  return 0;
}
