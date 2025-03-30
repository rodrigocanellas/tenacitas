/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tnct/traits/test/async_test.h"
#include "tnct/traits/test/is_tuple_test.h"
#include "tnct/traits/test/is_type_in_tuple_test.h"
#include "tnct/traits/test/string_test.h"
#include "tnct/traits/test/tuple_contains_tuple_test.h"
#include "tnct/traits/test/tuple_find_test.h"
#include "tnct/traits/test/tuple_like_test.h"
#include "tnct/traits/test/tuple_traverse_test.h"

#include "tnct/tester/test.h"

using namespace tnct;

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

    run_test(_tester, traits::test::tuple_traverse_000);

    run_test(_tester, traits::test::dispatcher_000);

    run_test(_tester, traits::test::has_publish_method_000);
    run_test(_tester, traits::test::has_publish_method_001);
    run_test(_tester, traits::test::has_publish_method_002);

    run_test(_tester, traits::test::has_add_handling_method_000);
    run_test(_tester, traits::test::has_add_handling_method_001);
    run_test(_tester, traits::test::has_add_handling_method_002);
    run_test(_tester, traits::test::has_add_handling_method_003);
    run_test(_tester, traits::test::has_add_handling_method_004);
    run_test(_tester, traits::test::has_add_handling_method_006);

    run_test(_tester, traits::test::handling_priority_000);
    run_test(_tester, traits::test::handling_priority_001);
    run_test(_tester, traits::test::handling_priority_002);

    run_test(_tester, traits::test::fixed_size_string_000);
    run_test(_tester, traits::test::fixed_size_string_001);

    run_test(_tester, traits::test::handler_000);
    run_test(_tester, traits::test::handler_001);
    run_test(_tester, traits::test::handler_002);
    run_test(_tester, traits::test::handler_003);
    run_test(_tester, traits::test::handler_004);
    run_test(_tester, traits::test::handler_005);

    run_test(_tester, traits::test::event_000);
    run_test(_tester, traits::test::event_001);
    run_test(_tester, traits::test::event_002);
    run_test(_tester, traits::test::event_003);
    run_test(_tester, traits::test::event_004);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
