/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/tst/tuple/tuple_transform_test.h"
#include "tenacitas/tst/tuple/tuple_traverse_test.h"

#include "tenacitas/src/tester/test.h"

using namespace tenacitas;

int main(int argc, char **argv) {
  src::tester::test _tester(argc, argv);

  run_test(_tester, tst::tuple::tuple_transform_000);
  run_test(_tester, tst::tuple::tuple_transform_001);

  run_test(_tester, tst::tuple::tuple_traverse_000);
  run_test(_tester, tst::tuple::tuple_traverse_001);
  run_test(_tester, tst::tuple::tuple_traverse_003);
  run_test(_tester, tst::tuple::tuple_traverse_004);
}
