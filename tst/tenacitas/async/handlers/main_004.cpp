/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory
/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <tst/tenacitas/async/handlers/test_base.h>

struct test_004{
    typedef test_t<4/* msg id */, 10/* num senders */, 
1/* msgs per sender */, 150 /* sender interval (ms) */,
1/* num handlers */,
400 /* timeout for each handler (ms) */,
3/* handler timeout at each */> test;

static std::string desc() { return test::description(); }

bool operator()() { test _test; return _test(); } };

    int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test<> _test(argc, argv);
    run_test(_test, test_004);
}
