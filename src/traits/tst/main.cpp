/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include "tenacitas/src/tester/test.h"
// #include "tenacitas/src/traits/tst/async_test.h"
#include "tenacitas/src/traits/tst/is_tuple_test.h"
#include "tenacitas/src/traits/tst/is_type_in_tuple_test.h"
// #include "tenacitas/src/traits/tst/string_test.h"
#include "tenacitas/src/traits/tst/tuple_contains_tuple_test.h"
#include "tenacitas/src/traits/tst/tuple_find_test.h"
#include "tenacitas/src/traits/tst/tuple_like_test.h"
#include "tenacitas/src/traits/tst/tuple_traverse_test.h"

int main(int argc, char **argv)
{

  try
  {
    src::tester::test _tester(argc, argv);

    run_test(_tester, src::traits::tst::is_tuple_000);
    run_test(_tester, src::traits::tst::is_tuple_001);
    run_test(_tester, src::traits::tst::is_tuple_002);
    run_test(_tester, src::traits::tst::is_tuple_003);
    run_test(_tester, src::traits::tst::is_tuple_004);
    run_test(_tester, src::traits::tst::is_tuple_005);
    run_test(_tester, src::traits::tst::is_tuple_006);

    run_test(_tester, src::traits::tst::tuple_like_000);
    run_test(_tester, src::traits::tst::tuple_like_001);
    run_test(_tester, src::traits::tst::tuple_like_002);

    run_test(_tester, src::traits::tst::tuple_find_000);
    run_test(_tester, src::traits::tst::tuple_find_001);
    run_test(_tester, src::traits::tst::tuple_find_002);

    run_test(_tester, src::traits::tst::tuple_contains_tuple_000);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_001);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_002);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_003);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_004);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_005);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_005);
    run_test(_tester, src::traits::tst::tuple_contains_tuple_007);

    run_test(_tester, src::traits::tst::is_type_in_tuple_000);
    run_test(_tester, src::traits::tst::is_type_in_tuple_001);
    run_test(_tester, src::traits::tst::is_type_in_tuple_002);

    run_test(_tester, src::traits::tst::tuple_traverse_000);

    // async
    // run_test(_tester, src::traits::tst::event_000);
    // run_test(_tester, src::traits::tst::event_001);
    // run_test(_tester, src::traits::tst::event_002);
    // run_test(_tester, src::traits::tst::event_003);
    // run_test(_tester, src::traits::tst::event_004);
    // run_test(_tester, src::traits::tst::event_005);

    // run_test(_tester, src::traits::tst::handler_000);
    // run_test(_tester, src::traits::tst::handler_001);
    // run_test(_tester, src::traits::tst::handler_002);
    // run_test(_tester, src::traits::tst::handler_003);
    // run_test(_tester, src::traits::tst::handler_004);
    // run_test(_tester, src::traits::tst::handler_005);

    // run_test(_tester, src::traits::tst::dispatcher_000);

    // run_test(_tester, src::traits::tst::has_publish_method_000);
    // run_test(_tester, src::traits::tst::has_publish_method_001);
    // run_test(_tester, src::traits::tst::has_publish_method_002);

    // run_test(_tester, src::traits::tst::has_add_handling_method_000);
    // run_test(_tester, src::traits::tst::has_add_handling_method_001);

    // run_test(_tester, src::traits::tst::fixed_size_string_000);
    // run_test(_tester, src::traits::tst::fixed_size_string_001);

    // run_test(_tester, src::traits::tst::handler_000);
    // run_test(_tester, src::traits::tst::handler_001);
    // run_test(_tester, src::traits::tst::handler_002);
    // run_test(_tester, src::traits::tst::handler_003);
    // run_test(_tester, src::traits::tst::handler_004);
    // run_test(_tester, src::traits::tst::handler_005);
  }
  catch (std::exception &_ex)
  {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
