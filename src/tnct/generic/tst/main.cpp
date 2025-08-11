/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tnct/generic/tst/id_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  try
  {
    tester::test _test(argc, argv);
    run_test(_test, generic::tst::id_001);
    run_test(_test, generic::tst::id_002);
    run_test(_test, generic::tst::id_003);
    run_test(_test, generic::tst::id_004);
    run_test(_test, generic::tst::id_005);
  }
  catch (std::exception &_ex)
  {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
