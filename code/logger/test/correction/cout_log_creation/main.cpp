#include <iostream>
#include <sstream>
#include <string>

#include <business/calendar/epoch.h>
#include <business/logger/cerr.h>
#include <business/logger/cout.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

struct cout_log_creation
{
  bool operator()()
  {

    try {

      logger::configure_cout_log();

      logger::log::set_debug();

      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cout_log_creation, argc, argv, "'cout_log' creation");
}
