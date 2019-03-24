#include <iostream>
#include <sstream>
#include <string>

#include <calendar/bus/epoch.h>
#include <logger/bus/cerr.h>
#include <logger/bus/cout.h>
#include <tester/bus/run.h>

class cerr_log_creation
{

  public:
    bool operator()()
    {

        try {
            tenacitas::logger::bus::configure_cerr_log();

            cerr_set_debug();

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
