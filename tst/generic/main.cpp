/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas/src/tester/test.h"
#include "tenacitas/tst/generic/id_test.h"

using namespace tenacitas;

int main(int argc, char **argv) {
  try {
    src::tester::test _test(argc, argv);
    run_test(_test, id_001);
    run_test(_test, id_002);
    run_test(_test, id_003);
    run_test(_test, id_004);
    run_test(_test, id_005);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
