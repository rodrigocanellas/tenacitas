#include <iostream>
#include <sstream>
#include <string>

#include <business/calendar/epoch.h>
#include <business/logger/cerr.h>
#include <business/logger/cout.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

class cerr_log_creation
{

public:
  bool operator()()
  {

    try {
      logger::configure_cerr_log();

      logger::log::set_debug();

      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

private:
  std::string m_base_cout_name;
};

int
main(int argc, char** argv)
{
  run_test(cerr_log_creation, argc, argv, "'cerr_log' creation");
}
