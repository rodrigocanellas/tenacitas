/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tnct/tester/test.h"

#include "tnct/tester/tst/test_test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  try
  {
    tester::test _test(argc, argv);
    run_test(_test, tester::tst::tester_ok);
    run_test(_test, tester::tst::tester_fail);
    run_test(_test, tester::tst::tester_error);
  }
  catch (std::exception &_ex)
  {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
