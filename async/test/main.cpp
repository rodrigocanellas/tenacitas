/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/async/test/dispatcher_test.h>
#include <tenacitas.lib/async/test/handling_id_test.h>
#include <tenacitas.lib/async/test/handling_test.h>
#include <tenacitas.lib/async/test/handlings_test.h>
#include <tenacitas.lib/async/test/sleeping_loop_test.h>
// #include <tenacitas.lib/generic/tester.h>

int main(int argc, char **argv) {

  generic::tester _tester(argc, argv);

  run_test(_tester, handling_000);
  run_test(_tester, handling_001);
  run_test(_tester, handling_005);
  run_test(_tester, handling_006);
  run_test(_tester, handling_007);
  run_test(_tester, handling_008);
  run_test(_tester, handling_009);
  run_test(_tester, handling_010);
  run_test(_tester, handling_011);
  run_test(_tester, handling_012);
  run_test(_tester, handling_014);
  run_test(_tester, handling_015);
  run_test(_tester, handling_016);
  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
  // run_test(_tester, handlings_000);
  // run_test(_tester, handlings_001);
  // run_test(_tester, handlings_002);
  // run_test(_tester, handlings_003);
  // run_test(_tester, handlings_004);
  // run_test(_tester, handlings_005);
  // run_test(_tester, handlings_006);
  // run_test(_tester, handlings_007);
  // run_test(_tester, handlings_001);
  // run_test(_tester, handlings_002);
  // run_test(_tester, handlings_003);
  // run_test(_tester, handlings_004);
  // run_test(_tester, handlings_005);
  run_test(_tester, dispatcher_000);
  // run_test(_tester, dispatcher_001);
  // run_test(_tester, dispatcher_002);
  // run_test(_tester, dispatcher_003);
  // run_test(_tester, dispatcher_004);
  // run_test(_tester, dispatcher_005);
  // run_test(_tester, dispatcher_006);
  // run_test(_tester, dispatcher_007);
  run_test(_tester, handling_id_000);
  run_test(_tester, handling_id_001);
  run_test(_tester, handling_id_002);
  run_test(_tester, handling_id_003);
  run_test(_tester, handling_id_004);
  run_test(_tester, handling_id_005);
  run_test(_tester, handling_id_006);
  run_test(_tester, handling_id_007);

  return 0;
}
