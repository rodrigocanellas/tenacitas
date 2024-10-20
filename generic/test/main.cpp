/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include <tenacitas.lib/generic/test/id_test.h>
#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/tester/test.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {
  try {
    generic::test _test(argc, argv);
    run_test(_test, id_001);
    run_test(_test, id_002);
    run_test(_test, id_003);
    run_test(_test, id_004);
    run_test(_test, id_005);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
