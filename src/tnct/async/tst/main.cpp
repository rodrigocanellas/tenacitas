/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/async/tst/cpt_test.h"
#include "tnct/async/tst/dispatcher_test.h"
#include "tnct/async/tst/handling_test.h"
#include "tnct/async/tst/sleeping_loop_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  tester::test _tester(argc, argv);

  run_test(_tester, async::tst::handling_000);
  run_test(_tester, async::tst::handling_001);
  run_test(_tester, async::tst::handling_005);
  run_test(_tester, async::tst::handling_006);
  run_test(_tester, async::tst::handling_007);
  run_test(_tester, async::tst::handling_008);
  run_test(_tester, async::tst::handling_009);
  run_test(_tester, async::tst::handling_010);
  run_test(_tester, async::tst::handling_011);
  run_test(_tester, async::tst::handling_014);
  run_test(_tester, async::tst::handling_015);
  run_test(_tester, async::tst::handling_016);

  run_test(_tester, async::tst::sleeping_loop_000);
  run_test(_tester, async::tst::sleeping_loop_001);
  run_test(_tester, async::tst::sleeping_loop_002);

  run_test(_tester, async::tst::dispatcher_000);
  run_test(_tester, async::tst::dispatcher_001);
  run_test(_tester, async::tst::dispatcher_002);
  run_test(_tester, async::tst::dispatcher_003);
  run_test(_tester, async::tst::dispatcher_007);
  run_test(_tester, async::tst::dispatcher_008);
  run_test(_tester, async::tst::dispatcher_009);

  // cpt
  run_test(_tester, async::tst::cpt_event_000);
  run_test(_tester, async::tst::cpt_event_001);
  run_test(_tester, async::tst::cpt_event_002);
  run_test(_tester, async::tst::cpt_event_003);
  run_test(_tester, async::tst::cpt_event_004);
  run_test(_tester, async::tst::cpt_event_005);

  run_test(_tester, async::tst::cpt_add_handler_000);
  run_test(_tester, async::tst::cpt_add_handler_001);
  run_test(_tester, async::tst::cpt_add_handler_002);
  run_test(_tester, async::tst::cpt_add_handler_003);
  run_test(_tester, async::tst::cpt_add_handler_004);
  run_test(_tester, async::tst::cpt_add_handler_005);

  run_test(_tester, async::tst::cpt_dispatcher_000);

  run_test(_tester, async::tst::cpt_has_publish_method_000);
  run_test(_tester, async::tst::cpt_has_publish_method_001);
  run_test(_tester, async::tst::cpt_has_publish_method_002);

  run_test(_tester, async::tst::cpt_has_add_handling_method_000);
  run_test(_tester, async::tst::cpt_has_add_handling_method_001);

  run_test(_tester, async::tst::cpt_handler_001);
  run_test(_tester, async::tst::cpt_handler_002);
  run_test(_tester, async::tst::cpt_handler_003);
  run_test(_tester, async::tst::cpt_handler_004);
  run_test(_tester, async::tst::cpt_handler_005);

  return 0;
}
