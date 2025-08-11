/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/tester/test.h"
#include "tnct/tuple/tst/contains_tuple_test.h"
#include "tnct/tuple/tst/contains_type_test.h"
#include "tnct/tuple/tst/get_type_index_test.h"
#include "tnct/tuple/tst/is_tuple_test.h"
#include "tnct/tuple/tst/transform_test.h"
#include "tnct/tuple/tst/traverse_test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  tester::test _tester(argc, argv);
  run_test(_tester, tuple::tst::cpt_contains_tuple_000);
  run_test(_tester, tuple::tst::cpt_contains_tuple_001);
  run_test(_tester, tuple::tst::cpt_contains_tuple_002);
  run_test(_tester, tuple::tst::cpt_contains_tuple_003);
  run_test(_tester, tuple::tst::cpt_contains_tuple_004);
  run_test(_tester, tuple::tst::cpt_contains_tuple_005);
  run_test(_tester, tuple::tst::cpt_contains_tuple_005);
  run_test(_tester, tuple::tst::cpt_contains_tuple_007);

  run_test(_tester, tuple::tst::cpt_is_tuple_000);
  run_test(_tester, tuple::tst::cpt_is_tuple_001);
  run_test(_tester, tuple::tst::cpt_is_tuple_002);

  run_test(_tester, tuple::tst::contains_type_000);
  run_test(_tester, tuple::tst::contains_type_001);
  run_test(_tester, tuple::tst::contains_type_002);

  run_test(_tester, tuple::tst::get_type_index_000);
  run_test(_tester, tuple::tst::get_type_index_001);
  run_test(_tester, tuple::tst::get_type_index_002);
  run_test(_tester, tuple::tst::get_type_index_003);
  run_test(_tester, tuple::tst::get_type_index_004);

  run_test(_tester, tuple::tst::transform_000);
  run_test(_tester, tuple::tst::transform_001);

  run_test(_tester, tuple::tst::traverse_000);
}
