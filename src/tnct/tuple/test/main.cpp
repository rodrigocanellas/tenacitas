/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/tuple/test/tuple_transform_test.h"
#include "tnct/tuple/test/tuple_traverse_test.h"

#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, tuple::test::tuple_transform_000);
  run_test(_tester, tuple::test::tuple_transform_001);

  run_test(_tester, tuple::test::tuple_traverse_000);
  run_test(_tester, tuple::test::tuple_traverse_001);
  run_test(_tester, tuple::test::tuple_traverse_002);
}
