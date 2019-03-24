#include <iostream>
#include <sstream>
#include <string>

#include <calendar/bus/epoch.h>
#include <logger/bus/cerr.h>
#include <logger/bus/cout.h>
#include <tester/bus/run.h>

struct cout_log_creation
{
    bool operator()()
    {

        try {
            tenacitas::logger::bus::configure_cout_log();

            cout_set_debug();

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO cout_log_creation: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cout_log_creation, argc, argv, "'cout_log' creation");
}
