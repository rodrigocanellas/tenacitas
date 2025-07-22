/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas/src/tester/test.h"

#include "tenacitas/tst/tester/test_test.h"

using namespace tenacitas;

int main(int argc, char **argv) {
  try {
    src::tester::test _test(argc, argv);
    run_test(_test, tester_ok);
    run_test(_test, tester_fail);
    run_test(_test, tester_error);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
