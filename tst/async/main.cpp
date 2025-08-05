/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/tester/test.h"
#include "tenacitas/tst/async/dispatcher_test.h"
#include "tenacitas/tst/async/handling_test.h"
#include "tenacitas/tst/async/sleeping_loop_test.h"
#include "tenacitas/tst/async/traits_test.h"

using namespace tenacitas;

int main(int argc, char **argv)
{
  src::tester::test _tester(argc, argv);

  run_test(_tester, tst::async::handling_000);
  run_test(_tester, tst::async::handling_001);
  run_test(_tester, tst::async::handling_005);
  run_test(_tester, tst::async::handling_006);
  run_test(_tester, tst::async::handling_007);
  run_test(_tester, tst::async::handling_008);
  run_test(_tester, tst::async::handling_009);
  run_test(_tester, tst::async::handling_010);
  run_test(_tester, tst::async::handling_011);
  run_test(_tester, tst::async::handling_014);
  run_test(_tester, tst::async::handling_015);
  run_test(_tester, tst::async::handling_016);

  run_test(_tester, tst::async::sleeping_loop_000);
  run_test(_tester, tst::async::sleeping_loop_001);
  run_test(_tester, tst::async::sleeping_loop_002);

  run_test(_tester, tst::async::dispatcher_000);
  run_test(_tester, tst::async::dispatcher_001);
  run_test(_tester, tst::async::dispatcher_002);
  run_test(_tester, tst::async::dispatcher_003);
  run_test(_tester, tst::async::dispatcher_007);
  run_test(_tester, tst::async::dispatcher_008);
  run_test(_tester, tst::async::dispatcher_009);

  // traits
  run_test(_tester, tst::async::traits::event_000);
  run_test(_tester, tst::async::traits::event_001);
  run_test(_tester, tst::async::traits::event_002);
  run_test(_tester, tst::async::traits::event_003);
  run_test(_tester, tst::async::traits::event_004);
  run_test(_tester, tst::async::traits::event_005);

  run_test(_tester, tst::async::traits::add_handler_000);
  run_test(_tester, tst::async::traits::add_handler_001);
  run_test(_tester, tst::async::traits::add_handler_002);
  run_test(_tester, tst::async::traits::add_handler_003);
  run_test(_tester, tst::async::traits::add_handler_004);
  run_test(_tester, tst::async::traits::add_handler_005);

  run_test(_tester, tst::async::traits::dispatcher_000);

  run_test(_tester, tst::async::traits::has_publish_method_000);
  run_test(_tester, tst::async::traits::has_publish_method_001);
  run_test(_tester, tst::async::traits::has_publish_method_002);

  run_test(_tester, tst::async::traits::has_add_handling_method_000);
  run_test(_tester, tst::async::traits::has_add_handling_method_001);

  run_test(_tester, tst::async::traits::handler_001);
  run_test(_tester, tst::async::traits::handler_002);
  run_test(_tester, tst::async::traits::handler_003);
  run_test(_tester, tst::async::traits::handler_004);
  run_test(_tester, tst::async::traits::handler_005);

  return 0;
}
