/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/tester/test.h"
#include "tnct/time/test/chrono_output_test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, time::test::time_output_000);
  run_test(_tester, time::test::time_output_001);
  run_test(_tester, time::test::time_output_002);
  run_test(_tester, time::test::time_output_003);
  run_test(_tester, time::test::time_output_004);
  run_test(_tester, time::test::time_output_005);
  run_test(_tester, time::test::time_output_006);
  run_test(_tester, time::test::time_output_007);
  run_test(_tester, time::test::time_output_008);
}
