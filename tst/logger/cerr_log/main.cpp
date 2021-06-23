/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <sstream>
#include <string>

#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct cerr_log_how_to {
    bool operator()() {

        try {

            DEB("hello! ", 309);
            DEB("how are you doing? ", 3.14);
            INF("fine!! ", 'W');
            INF("and you?");
            WAR("great! got a new job!! ", 6987.58f);
            WAR("nice!! ", 10);
            return true;
        } catch (std::exception &_ex) {
            std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }

    static std::string desc() { return "Simple 'cerr' log usage"; }
};

int main(int argc, char **argv) {

    logger::set_writer_cerr();
    logger::set_debug_level();
    tester::test<> _tester(argc, argv);
    run_test(_tester, cerr_log_how_to);
}
