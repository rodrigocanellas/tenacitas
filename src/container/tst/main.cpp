/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/tester/test.h"

#include "tenacitas/src/container/tst/circular_queue_test.h"
#include "tenacitas/src/container/tst/matrix_test.h"

using namespace tenacitas;

int main(int argc, char **argv)
{

  src::tester::test _tester(argc, argv);
  run_test(_tester, src::container::tst::circular_queue_001);
  run_test(_tester, src::container::tst::circular_queue_003);
  run_test(_tester, src::container::tst::circular_queue_test);

  run_test(_tester, src::container::tst::matrix_000);
  run_test(_tester, src::container::tst::matrix_001);
  run_test(_tester, src::container::tst::matrix_002);
  run_test(_tester, src::container::tst::matrix_003);
  run_test(_tester, src::container::tst::matrix_004);
}
