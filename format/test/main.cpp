/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas.lib/format/test/format_number_test.h"
#include "tenacitas.lib/tester/test.h"
#include "tenacitas.lib/program/options.h"

using namespace tenacitas::lib;

int main(int argc, char **argv) {
  try {
    tester::test _test(argc, argv);
    run_test(_test, format_number_000);
    run_test(_test, format_number_001);
    run_test(_test, format_number_002);
    run_test(_test, format_number_003);
    run_test(_test, format_number_004);
    run_test(_test, format_number_005);
    run_test(_test, format_number_006);
    run_test(_test, format_number_009);
    run_test(_test, format_number_010);
    run_test(_test, format_number_011);
    run_test(_test, format_number_012);
    run_test(_test, format_number_014);
    run_test(_test, format_number_015);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
