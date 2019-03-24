#include <iostream>
#include <sstream>
#include <string>

#include <calendar/bus/epoch.h>
#include <logger/bus/file.h>
#include <tester/bus/run.h>

class file_log_creation
{

  public:
    explicit file_log_creation() = default;

    bool operator()()
    {

        try {
            tenacitas::logger::bus::configure_file_log(
              ".", "file_log_creation", 100, std::chrono::minutes(10));

            file_set_debug();

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO file_log_creation: '", _ex.what(), "'");
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
