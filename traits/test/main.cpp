/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include <tenacitas.lib/generic/tester.h>
#include <tenacitas.lib/traits/test/subscriber.h>
#include <tenacitas.lib/traits/test/tuple.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {

  try {
    generic::tester _test(argc, argv);
    run_test(_test, tuple_000);
    run_test(_test, tuple_001);
    run_test(_test, tuple_002);
    run_test(_test, tuple_003);
    run_test(_test, tuple_004);
    run_test(_test, tuple_005);
    run_test(_test, tuple_006);
    run_test(_test, pair_000);
    run_test(_test, tuple_pair_000);
    run_test(_test, subscriber_000);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
