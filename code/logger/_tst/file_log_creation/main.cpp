#include <iostream>
#include <sstream>
#include <string>

#include <calendar/_bus/epoch.h>
#include <logger/file/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

class file_log_creation {

public:
  explicit file_log_creation() = default;

  bool operator()() {

    try {
      logger::file::log::configure(".", "file_log_creation", 100,
                                   std::chrono::minutes(10));

      logger::file::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      logger::file::log::fatal("file_log_creation", __LINE__,
                               "ERRO file_log_creation: '", _ex.what(), "'");
    }
    return false;
  }
  static std::string desc() { return "'file_log' creation"; }
  static std::string name() { return "file_log_creation"; }
};

int main(int argc, char **argv) {
  logger::file::log::set_debug();
  tester::_bus::test::run<file_log_creation>(argc, argv);
}
