/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/tester/test.h>
#include <tenacitas.lib/tester/test/test_test.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {
  try {
    tester::test _test(argc, argv);
    run_test(_test, tester_ok);
    run_test(_test, tester_fail);
    run_test(_test, tester_error);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
