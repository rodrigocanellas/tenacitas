#include <iostream>
#include <sstream>
#include <string>

#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

class cerr_log_creation {

public:
  bool operator()() {
    using namespace tenacitas;
    try {

      logger::cerr::log::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "'cerr' log creation"; }

  static std::string name() { return "cerr_log_creation"; }

private:
  std::string m_base_cout_name;
};

int main(int argc, char **argv) {
  tester::bus::test::run<cerr_log_creation>(argc, argv);
}
