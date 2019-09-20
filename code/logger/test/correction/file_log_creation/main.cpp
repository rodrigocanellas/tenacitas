#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <logger/business/file.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

class file_log_creation
{

  public:
    explicit file_log_creation() = default;

    bool operator()()
    {
        using namespace tenacitas::logger::business;
        try {
            configure_file_log(
              ".", "file_log_creation", 100, std::chrono::minutes(10));

            log::set_debug();

            return true;
        } catch (std::exception& _ex) {
            log::fatal("file_log_creation",
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
