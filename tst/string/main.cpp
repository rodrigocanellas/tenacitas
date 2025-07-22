/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/tester/test.h"

#include "tenacitas/tst/string/fixed_size_string_test.h"

using namespace tenacitas;

int main(int argc, char **argv) {
  src::tester::test _tester(argc, argv);

  run_test(_tester, tst::string::fixed_size_string_000);
  run_test(_tester, tst::string::fixed_size_string_001);
  run_test(_tester, tst::string::fixed_size_string_002);
  run_test(_tester, tst::string::fixed_size_string_003);
  run_test(_tester, tst::string::fixed_size_string_004);
  run_test(_tester, tst::string::fixed_size_string_005);
}
