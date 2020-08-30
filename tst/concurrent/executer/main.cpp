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

struct executer_000 {
  bool operator()() {

    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, _provider, std::chrono::milliseconds(_timeout));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    return _executer.is_stopped();
  }

  static std::string desc() {
    return "Stopping an executer with parameters that was not called";
  }

private:
  logger::cerr::log m_log{"executer_000"};
};

struct executer_001 {
  bool operator()() {

    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, _provider, std::chrono::milliseconds(_timeout));

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
    return "Stopping an executer with parameters that was not called, and it "
           "was already stopped";
  }

private:
  logger::cerr::log m_log{"executer_001"};
};

struct executer_002 {
  bool operator()() {

    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, _provider, std::chrono::milliseconds(_timeout));

    if (!_executer.is_started()) {
      concurrent_error(m_log, "it should be running, but it is not");
      return false;
    }

    _executer.start();

    return _executer.is_started();
  }

  static std::string desc() {
    return "Starting an executer with parameters that is started";
  }

private:
  logger::cerr::log m_log{"executer_002"};
};

struct executer_003 {
  bool operator()() {

    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    const auto _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, _provider, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "starting again");
    _executer.start();

    _thread.join();

    return _executer.is_started();
  }

  static std::string desc() {
    return "Starting an executer with parameters that is started, and after "
           "call to 'operator()()";
  }

private:
  logger::cerr::log m_log{"executer_003"};
};

struct executer_004 {
  bool operator()() {
    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    const uint16_t _timeout = 6000;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      concurrent_debug(m_log, "sleeping");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 2)));
      concurrent_debug(m_log, "waking");

      return true;
    };

    typename executer::provider _provider =
        []() -> std::optional<std::tuple<int, double>> {
      return {{-9, 3.14}};
    };

    executer _executer(_worker, _provider, std::chrono::milliseconds(_timeout));

    std::thread _thread([&_executer]() -> void { _executer(); });

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<uint16_t>(_timeout / 4)));

    concurrent_debug(m_log, "stopping");
    _executer.stop();

    _thread.join();

    return true;
  }

  static std::string desc() {
    return "Stopping an executer with parameters that was called";
  }

private:
  logger::cerr::log m_log{"executer_001"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, executer_000);
  run_test(_test, executer_001);
  run_test(_test, executer_002);
  run_test(_test, executer_003);
}
