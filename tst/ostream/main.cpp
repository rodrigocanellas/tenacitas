
/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/tst/ostream/pair_output_test.h"
#include "tenacitas/tst/ostream/tuple_output_test.h"

#include "tenacitas/src/tester/test.h"

using namespace tenacitas;

int main(int argc, char **argv) {
  src::tester::test _tester(argc, argv);

  run_test(_tester, tst::ostream::tuple_output_000);
  run_test(_tester, tst::ostream::tuple_output_001);
  run_test(_tester, tst::ostream::tuple_output_002);

  run_test(_tester, tst::ostream::pair_output_000);
  run_test(_tester, tst::ostream::pair_output_000);
}
