/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/generic/tester.h>
#include <tenacitas.lib/container/test/matrix.h>
#include <tenacitas.lib/container/test/test_circular_queue.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {

  generic::tester _tester(argc, argv);
  run_test(_tester, circular_queue_001);
  run_test(_tester, circular_queue_003);
  run_test(_tester, circular_queue_test);

  run_test(_tester, matrix_000);
  run_test(_tester, matrix_001);
  run_test(_tester, matrix_002);
  run_test(_tester, matrix_003);
  run_test(_tester, matrix_004);
}
