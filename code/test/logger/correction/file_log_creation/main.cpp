#include <iostream>
#include <sstream>
#include <string>

#include <business/calendar/epoch.h>
#include <business/logger/file.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

class file_log_creation
{

public:
  explicit file_log_creation() = default;

  bool operator()()
  {

    try {
      logger::configure_file_log(
            ".", "file_log_creation", 100, std::chrono::minutes(10));

      logger::log::set_debug();

      return true;
    } catch (std::exception& _ex) {
      logger::log::fatal("file_log_creation",
                         __LINE__,
                         "ERRO file_log_creation: '",
                         _ex.what(),
                         "'");
    }
    return false;
  }

private:
  std::string m_base_file_name;
};

int
main(int argc, char** argv)
{
  run_test(file_log_creation, argc, argv, "'file_log' creation");
}
