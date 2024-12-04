/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas.lib/traits/test/is_tuple_test.h"
#include "tenacitas.lib/traits/test/is_type_in_tuple_test.h"
#include "tenacitas.lib/traits/test/tuple_contains_tuple_test.h"
#include "tenacitas.lib/traits/test/tuple_find_test.h"
#include "tenacitas.lib/traits/test/tuple_like_test.h"

#include "tenacitas.lib/tester/test.h"

using namespace tenacitas::lib;

int main(int argc, char **argv) {

  try {
    tester::test _tester(argc, argv);

    run_test(_tester, traits::test::is_tuple_000);
    run_test(_tester, traits::test::is_tuple_001);
    run_test(_tester, traits::test::is_tuple_002);
    run_test(_tester, traits::test::is_tuple_003);
    run_test(_tester, traits::test::is_tuple_004);
    run_test(_tester, traits::test::is_tuple_005);
    run_test(_tester, traits::test::is_tuple_006);

    run_test(_tester, traits::test::tuple_like_000);
    run_test(_tester, traits::test::tuple_like_001);
    run_test(_tester, traits::test::tuple_like_002);

    run_test(_tester, traits::test::tuple_find_000);
    run_test(_tester, traits::test::tuple_find_001);
    run_test(_tester, traits::test::tuple_find_002);

    run_test(_tester, traits::test::tuple_contains_tuple_000);
    run_test(_tester, traits::test::tuple_contains_tuple_001);
    run_test(_tester, traits::test::tuple_contains_tuple_002);
    run_test(_tester, traits::test::tuple_contains_tuple_003);
    run_test(_tester, traits::test::tuple_contains_tuple_004);
    run_test(_tester, traits::test::tuple_contains_tuple_005);
    run_test(_tester, traits::test::tuple_contains_tuple_005);
    run_test(_tester, traits::test::tuple_contains_tuple_007);

    run_test(_tester, traits::test::is_type_in_tuple_000);
    run_test(_tester, traits::test::is_type_in_tuple_001);
    run_test(_tester, traits::test::is_type_in_tuple_002);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
