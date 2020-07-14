
#include <cstdint>
#include <iostream>
#include <string>

#include <communication/client.h>
#include <tester/test.h>

using namespace tenacitas;

struct connection {};

struct send_to_file {
  bool operator()() {
    // insert your test code here
    return true;
  }

  static std::string desc() { return ""; }
};

int main(int argc, char **argv) {
  tester::test _test(argc, argv);
  run_test(_test, send_to_file);
}
