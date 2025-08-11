
/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/ostream/tst/pair_output_test.h"
#include "tnct/ostream/tst/tuple_output_test.h"

#include "tnct/tester/test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  tester::test _tester(argc, argv);

  run_test(_tester, ostream::tst::tuple_output_000);
  run_test(_tester, ostream::tst::tuple_output_001);
  run_test(_tester, ostream::tst::tuple_output_002);

  run_test(_tester, ostream::tst::pair_output_000);
  run_test(_tester, ostream::tst::pair_output_000);
}
