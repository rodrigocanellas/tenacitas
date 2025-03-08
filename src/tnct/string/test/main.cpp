/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/string/test/fixed_size_string_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, string::test::fixed_size_string_000);
  run_test(_tester, string::test::fixed_size_string_001);
  run_test(_tester, string::test::fixed_size_string_002);
  run_test(_tester, string::test::fixed_size_string_003);
}
