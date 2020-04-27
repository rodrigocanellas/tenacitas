
#include <cstdint>
#include <iostream>
#include <string>

#include <tester/test.h>

using namespace tenacitas;

struct test_ok {
  bool operator()() { return true; }
};

struct test_fail {
  bool operator()() { return true; }
};

struct test_error {
  bool operator()() {
    throw std::runtime_error("test function raised an exception");
  }
};

int main(int argc, char **argv) {
  try {
    tester::test _test(argc, argv);

    _test(test_ok(), "test_ok", "runs a correct test");
    _test(test_fail(), "test_fail", "runs a fail test");
    _test(test_error(), "test_error", "runs a error test");

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
