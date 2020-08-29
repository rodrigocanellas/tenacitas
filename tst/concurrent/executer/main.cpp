#include <cstdint>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include <concurrent/executer.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct executer_000 {
  bool operator()() {

    typedef concurrent::executer_t<std::chrono::milliseconds, logger::cerr::log,
                                   int16_t, double>
        executer;

    typename executer::worker _worker = [this](int16_t p_int,
                                               double p_double) -> bool {
      concurrent_debug(m_log, "int = ", p_int, ", double = ", p_double);
      return true;
    };

    executer _executer(_worker, std::chrono::milliseconds(5000));
    int16_t _i = -9;
    double _d = 3.1415;

    std::async([&_executer, _i, _d]() -> void { _executer(_i, _d); });

    _executer.stop();

    return true;
  }

  static std::string desc() { return "Creating a 'executer_t'"; }

private:
  logger::cerr::log m_log{"executer_000"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, executer_000);
}
