
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/weeks.h>
#include <tester/test.h>

using namespace tenacitas;

struct amount_basic_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      weeks _weeks;
      if (_weeks != weeks(0)) {
        return false;
      }
    }

    {
      weeks _weeks(8);
      if (_weeks != weeks(8)) {
        return false;
      }
    }

    {
      weeks _weeks(5);
      _weeks += weeks(8);
      if (_weeks != weeks(13)) {
        return false;
      }
    }

    return true;
  }

  static std::string desc() {
    return "Executes basic tests on 'weeks', a type of 'amount'";
  }

  static std::string name() { return "amount_basic_test"; }
};

int main(int argc, char **argv) {
  tester::test::run<amount_basic_test>(argc, argv);
}
