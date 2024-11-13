/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/tuple/test/tuple_output_test.h>
#include <tenacitas.lib/tuple/test/tuple_transform_test.h>
#include <tenacitas.lib/tuple/test/tuple_traverse_test.h>

#include <tenacitas.lib/tester/test.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, tuple::test::tuple_transform_000);
  run_test(_tester, tuple::test::tuple_transform_001);

  run_test(_tester, tuple::test::tuple_traverse_000);
  run_test(_tester, tuple::test::tuple_traverse_001);
  run_test(_tester, tuple::test::tuple_traverse_002);

  run_test(_tester, tuple::test::tuple_output_000);
  run_test(_tester, tuple::test::tuple_output_001);
  run_test(_tester, tuple::test::tuple_output_002);
}
