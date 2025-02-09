/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/container/test/circular_queue_test.h"
#include "tnct/container/test/matrix_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv) {

  tester::test _tester(argc, argv);
  run_test(_tester, circular_queue_001);
  run_test(_tester, circular_queue_003);
  run_test(_tester, circular_queue_test);

  run_test(_tester, matrix_000);
  run_test(_tester, matrix_001);
  run_test(_tester, matrix_002);
  run_test(_tester, matrix_003);
  run_test(_tester, matrix_004);
}
