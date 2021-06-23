/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct cout_log_how_to {
    bool operator()() {
        try {
            DEB("how are you doing? ", 3.14);
            INF("fine!! ", 'W');
            INF("and you?");
            WAR("great! got a new job!! ", 6987.58f);
            WAR("nice!! ", 10);
            return true;
        } catch (std::exception &_ex) {
            std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }

    static std::string desc() { return "Simple 'cout' log usage"; }
};

struct cout_log_print_tuple {
    static std::string desc() { return "Prints a tuple"; }

    bool operator()() {
        try {
            std::tuple<int16_t, float> _tuple {-3, 3.14};

            DEB("how are you doing? ", _tuple);
            return true;
        } catch (std::exception &_ex) {
            std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }
};

int main(int argc, char **argv) {
    logger::set_writer_cout();
    logger::set_debug_level();
    tester::test<> _tester(argc, argv);

    run_test(_tester, cout_log_how_to);
    run_test(_tester, cout_log_print_tuple);
}
