/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas/tst/traits/async_test.h"
#include "tenacitas/tst/traits/is_tuple_test.h"
#include "tenacitas/tst/traits/is_type_in_tuple_test.h"
#include "tenacitas/tst/traits/string_test.h"
#include "tenacitas/tst/traits/tuple_contains_tuple_test.h"
#include "tenacitas/tst/traits/tuple_find_test.h"
#include "tenacitas/tst/traits/tuple_like_test.h"
#include "tenacitas/tst/traits/tuple_traverse_test.h"

#include "tenacitas/src/tester/test.h"

using namespace tenacitas;

int main(int argc, char **argv) {

  try {
    src::tester::test _tester(argc, argv);

    run_test(_tester, tst::traits::is_tuple_000);
    run_test(_tester, tst::traits::is_tuple_001);
    run_test(_tester, tst::traits::is_tuple_002);
    run_test(_tester, tst::traits::is_tuple_003);
    run_test(_tester, tst::traits::is_tuple_004);
    run_test(_tester, tst::traits::is_tuple_005);
    run_test(_tester, tst::traits::is_tuple_006);

    run_test(_tester, tst::traits::tuple_like_000);
    run_test(_tester, tst::traits::tuple_like_001);
    run_test(_tester, tst::traits::tuple_like_002);

    run_test(_tester, tst::traits::tuple_find_000);
    run_test(_tester, tst::traits::tuple_find_001);
    run_test(_tester, tst::traits::tuple_find_002);

    run_test(_tester, tst::traits::tuple_contains_tuple_000);
    run_test(_tester, tst::traits::tuple_contains_tuple_001);
    run_test(_tester, tst::traits::tuple_contains_tuple_002);
    run_test(_tester, tst::traits::tuple_contains_tuple_003);
    run_test(_tester, tst::traits::tuple_contains_tuple_004);
    run_test(_tester, tst::traits::tuple_contains_tuple_005);
    run_test(_tester, tst::traits::tuple_contains_tuple_005);
    run_test(_tester, tst::traits::tuple_contains_tuple_007);

    run_test(_tester, tst::traits::is_type_in_tuple_000);
    run_test(_tester, tst::traits::is_type_in_tuple_001);
    run_test(_tester, tst::traits::is_type_in_tuple_002);

    run_test(_tester, tst::traits::tuple_traverse_000);

    run_test(_tester, tst::traits::dispatcher_000);

    run_test(_tester, tst::traits::has_publish_method_000);
    run_test(_tester, tst::traits::has_publish_method_001);
    run_test(_tester, tst::traits::has_publish_method_002);

    run_test(_tester, tst::traits::has_add_handling_method_000);
    run_test(_tester, tst::traits::has_add_handling_method_001);
    run_test(_tester, tst::traits::has_add_handling_method_002);
    run_test(_tester, tst::traits::has_add_handling_method_003);
    run_test(_tester, tst::traits::has_add_handling_method_004);
    run_test(_tester, tst::traits::has_add_handling_method_006);

    run_test(_tester, tst::traits::handling_priority_000);
    run_test(_tester, tst::traits::handling_priority_001);
    run_test(_tester, tst::traits::handling_priority_002);

    run_test(_tester, tst::traits::fixed_size_string_000);
    run_test(_tester, tst::traits::fixed_size_string_001);

    run_test(_tester, tst::traits::handler_000);
    run_test(_tester, tst::traits::handler_001);
    run_test(_tester, tst::traits::handler_002);
    run_test(_tester, tst::traits::handler_003);
    run_test(_tester, tst::traits::handler_004);
    run_test(_tester, tst::traits::handler_005);

    run_test(_tester, tst::traits::event_000);
    run_test(_tester, tst::traits::event_001);
    run_test(_tester, tst::traits::event_002);
    run_test(_tester, tst::traits::event_003);
    run_test(_tester, tst::traits::event_004);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
