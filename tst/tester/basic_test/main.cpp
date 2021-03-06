/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>
#include <string>

#include <tester/test.h>

using namespace tenacitas;

struct test_ok {
  bool operator()() { return true; }

  static std::string desc() { return "an ok test"; }
};

struct test_fail {
  bool operator()() { return true; }
  static std::string desc() { return "a fail test"; }
};

struct test_error {
  bool operator()() {
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

int main(int argc, char **argv) {
  try {
    tester::test _test(argc, argv);
    run_test(_test, test_ok);
    run_test(_test, test_fail);
    run_test(_test, test_error);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
