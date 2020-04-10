#include <iostream>
#include <sstream>
#include <string>

#include <logger/cout/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct cout_log_creation {
  bool operator()() {

    try {

      logger::cout::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "'cout' log creation"; }
  static std::string name() { return "cout_log_creation"; }
};

int main(int argc, char **argv) {
  tester::_bus::test::run<cout_log_creation>(argc, argv);
}
