/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

// #include <iostream>

#include "tnct/async/test/dispatcher_test.h"
#include "tnct/async/test/handling_test.h"
#include "tnct/async/test/sleeping_loop_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::test _test(argc, argv);

  run_test(_test, handling_000);
  run_test(_test, handling_001);
  run_test(_test, handling_005);
  run_test(_test, handling_006);
  run_test(_test, handling_007);
  run_test(_test, handling_008);
  run_test(_test, handling_009);
  run_test(_test, handling_010);
  run_test(_test, handling_011);
  // run_test(_test, handling_012);
  run_test(_test, handling_014);
  run_test(_test, handling_015);
  run_test(_test, handling_016);
  run_test(_test, sleeping_loop_000);
  run_test(_test, sleeping_loop_001);
  run_test(_test, sleeping_loop_002);
  run_test(_test, dispatcher_000);
  run_test(_test, dispatcher_001);
  run_test(_test, dispatcher_002);
  run_test(_test, dispatcher_003);
  run_test(_test, dispatcher_007);
  run_test(_test, dispatcher_008);
  run_test(_test, dispatcher_011);
  run_test(_test, dispatcher_012);

  return 0;
}
