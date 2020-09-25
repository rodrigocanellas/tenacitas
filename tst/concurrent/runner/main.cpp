#include <chrono>
#include <cstdint>
#include <iostream>

#include <concurrent/runner.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct runner_000 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds,
                                 int16_t>
        runner;

    runner _runner(std::chrono::seconds(1), worker());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //    auto _result = _runner().value();

    auto _maybe = _runner();

    if (_maybe) {

      auto _result = _maybe.value();

      concurrent_debug(m_log, "result = ", _result);

      return _result == -65;
    }
    return false;
  }

  static std::string desc() {
    return "Runner returning int16_t, with no parameters";
  }

private:
  struct worker {
    int16_t operator()() { return -65; }
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_000"};
};

struct runner_001 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds,
                                 int16_t, int16_t>
        runner;

    runner _runner(std::chrono::seconds(1), worker());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto _maybe = _runner(10);
    if (_maybe) {

      auto _result = _maybe.value();

      concurrent_debug(m_log, "result = ", _result);

      return _result == 14;
    }
    return false;
  }

  static std::string desc() {
    return "Runner returning int16_t, with one int16_t parameter";
  }

private:
  struct worker {
    int16_t operator()(int16_t p_int) { return p_int + 4; }
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_001"};
};

struct runner_002 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds,
                                 double, int16_t, double>
        runner;

    runner _runner(std::chrono::seconds(1), worker());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto _maybe = _runner(11, 2.3);
    if (_maybe) {
      auto _result = _maybe.value();
      concurrent_debug(m_log, "result = ", _result);

      return _result == (2.3 * 11);
    }
    return false;
  }

  static std::string desc() {
    return "Runner returning double, with a int16_t and a double parameter";
  }

private:
  struct worker {
    double operator()(int16_t p_int, double p_double) {
      return p_double * static_cast<double>(p_int);
    }
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_002"};
};

struct runner_003 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds, void>
        runner;

    runner _runner(std::chrono::seconds(1), worker());

    _runner();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

  static std::string desc() {
    return "Runner returning void, with no parameters";
  }

private:
  struct worker {
    void operator()() { concurrent_debug(m_log, "work done!!!"); }

  private:
    tenacitas::logger::cerr::log m_log{"runner_003::worker"};
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_003"};
};

struct runner_004 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds, void,
                                 std::string, uint32_t>
        runner;

    auto _provider = []() -> std::optional<std::tuple<std::string, uint32_t>> {
      return {{"hello!!!", 135790}};
    };

    runner _runner(std::chrono::seconds(1), worker());

    std::apply(_runner, _provider().value());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

  static std::string desc() {
    return "Runner returning void, with parameters 'string' and uint32_t, "
           "provided by a function";
  }

private:
  struct worker {
    void operator()(std::string &&p_str, uint32_t p_int) {
      concurrent_debug(m_log, "str = '", p_str, "', int = ", p_int);
    }

  private:
    tenacitas::logger::cerr::log m_log{"runner_004::worker"};
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_004"};
};

struct runner_005 {

  bool operator()() {
    using namespace tenacitas;

    typedef concurrent::runner_t<logger::cerr::log, std::chrono::seconds,
                                 int16_t>
        runner;

    std::function<std::optional<std::tuple<>>()> _provider =
        []() -> std::optional<std::tuple<>> { return {std::tuple<>()}; };

    runner _runner(std::chrono::seconds(1), worker());

    auto _maybe = _provider();
    if (_maybe) {

      std::apply(_runner, _maybe.value());
      std::this_thread::sleep_for(std::chrono::seconds(1));

      return true;
    }
    return false;
  }

  static std::string desc() {
    return "Runner returning int16_t, with no parameters, and passing a "
           "provider function";
  }

private:
  struct worker {
    int16_t operator()() {
      concurrent_debug(m_log, "working!!");
      return 19;
    }

  private:
    tenacitas::logger::cerr::log m_log{"runner_005::worker"};
  };

private:
  tenacitas::logger::cerr::log m_log{"runner_005"};
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

  return 0;
}
