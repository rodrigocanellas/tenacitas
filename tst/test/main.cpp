/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>
#include <string>

#include <tenacitas.lib/src/program/alg/options.h>
#include <tenacitas.lib/src/test/alg/tester.h>

namespace tenacitas::lib::tst::test::alg {

struct test_ok {
  bool operator()(const src::program::alg::options &) { return true; }

  static std::string desc() { return "an ok test"; }
};

struct test_fail {
  bool operator()(const src::program::alg::options &) { return true; }
  static std::string desc() { return "a fail test"; }
};

struct test_error {
  bool operator()(const src::program::alg::options &) {
    try {
      throw std::runtime_error("test function raised an exception");
      return false;
    } catch (const std::exception &_ex) {
      std::cerr << "'test_error' raised '" << _ex.what() << "'" << std::endl;
      return true;
    }
  }
  static std::string desc() { return "an eror test"; }
};

} // namespace tenacitas::lib::tst::test::alg

int main(int argc, char **argv) {
  using namespace tenacitas;
  try {
    lib::src::test::alg::tester _test(argc, argv);
    run_test(_test, lib::tst::test::alg::test_ok);
    run_test(_test, lib::tst::test::alg::test_fail);
    run_test(_test, lib::tst::test::alg::test_error);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
