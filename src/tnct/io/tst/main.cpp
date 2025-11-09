/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/io/tst/cpt_test.h"
#include "tnct/tester/bus/test.h"

using namespace tnct;

int main(int argc, char **argv)
{
  tester::bus::test _tester(argc, argv);

  run_test(_tester, tnct::io::tst::cpt_reader_000);
  run_test(_tester, tnct::io::tst::cpt_reader_001);
  run_test(_tester, tnct::io::tst::cpt_reader_002);
  run_test(_tester, tnct::io::tst::cpt_reader_003);
  run_test(_tester, tnct::io::tst::cpt_reader_004);

  run_test(_tester, tnct::io::tst::cpt_writer_000);
  run_test(_tester, tnct::io::tst::cpt_writer_001);
  run_test(_tester, tnct::io::tst::cpt_writer_002);
  run_test(_tester, tnct::io::tst::cpt_writer_003);

  return 0;
}
