/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

struct clog_how_to {

    bool operator()() {
        try {
            using namespace tenacitas;
            DEB("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                3.1415);
            INF("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                3.1415);
            WAR("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                3.1415);
            ERR("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                3.1415);
            FAT("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                3.1415);

            return true;

        } catch (std::exception &_ex) {
            std::cout << "ERRO clog_how_to: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }

    static std::string desc() { return "Simple 'clog' log usage"; }
};

int main(int argc, char **argv) {
    logger::set_debug_level();
    logger::set_writer_clog();

    tester::test _tester(argc, argv);
    run_test(_tester, clog_how_to);
}
