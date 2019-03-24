#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <logger/business/cerr.h>
#include <logger/business/cout.h>
#include <tester/business/run.h>

class cerr_log_creation
{

  public:
    bool operator()()
    {

        try {
            tenacitas::logger::business::configure_cerr_log();

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
