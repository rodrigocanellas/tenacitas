/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/supplier/tst/cpt_test.h"
#include "tnct/tester/bus/test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::bus::test _tester(argc, argv);

  run_test(_tester, supplier::tst::cpt_test_001);
  run_test(_tester, supplier::tst::cpt_test_002);
  run_test(_tester, supplier::tst::cpt_test_003);
  run_test(_tester, supplier::tst::cpt_test_004);
  run_test(_tester, supplier::tst::cpt_test_005);
  run_test(_tester, supplier::tst::cpt_test_006);
  run_test(_tester, supplier::tst::cpt_test_007);
  run_test(_tester, supplier::tst::cpt_test_008);
  run_test(_tester, supplier::tst::cpt_test_009);
  run_test(_tester, supplier::tst::cpt_test_010);
  run_test(_tester, supplier::tst::cpt_test_011);
  run_test(_tester, supplier::tst::cpt_test_012);
  run_test(_tester, supplier::tst::cpt_test_013);
  run_test(_tester, supplier::tst::cpt_test_014);
}
