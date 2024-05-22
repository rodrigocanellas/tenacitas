/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/generic/program_options.h>
#include <tenacitas.lib/generic/tester.h>
#include <tenacitas.lib/test/math/factorial.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {

  generic::tester _tester(argc, argv);
  run_test(_tester, test0000);
  run_test(_tester, test0001);
}
