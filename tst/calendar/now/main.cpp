
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/timestamp.h>
#include <tester/test.h>

using namespace tenacitas;

struct now {
  bool operator()() {
    std::cerr << "now = " << calendar::gregorian::timestamp() << std::endl;
    return true;
  }

  static std::string desc() { return "Calculates 'now'"; }

  static std::string name() { return "now"; }
};

int main(int argc, char **argv) { tester::test::run<now>(argc, argv); }
