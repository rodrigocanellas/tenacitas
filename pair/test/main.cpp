/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas.lib/tester/test.h"
#include "tenacitas.lib/pair/test/pair_output_test.h"

using namespace tenacitas::lib;

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, pair::test::pair_output_000);
  run_test(_tester, pair::test::pair_output_001);
}
