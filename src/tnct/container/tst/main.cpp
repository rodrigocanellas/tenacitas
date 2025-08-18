/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/tester/test.h"

#include "tnct/container/tst/circular_queue_test.h"
#include "tnct/container/tst/cpt_test.h"
#include "tnct/container/tst/matrix_test.h"

using namespace tnct;

int main(int argc, char **argv)
{

  tester::test _tester(argc, argv);
  run_test(_tester, container::tst::circular_queue_001);
  run_test(_tester, container::tst::circular_queue_003);
  run_test(_tester, container::tst::circular_queue_test);

  run_test(_tester, container::tst::matrix_000);
  run_test(_tester, container::tst::matrix_001);
  run_test(_tester, container::tst::matrix_002);
  run_test(_tester, container::tst::matrix_003);
  run_test(_tester, container::tst::matrix_004);

  run_test(_tester, container::tst::cpt_test_000);
  run_test(_tester, container::tst::cpt_test_001);
  run_test(_tester, container::tst::cpt_test_002);
}
