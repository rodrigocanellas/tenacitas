
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
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

    {
      weeks _weeks(9);
      _weeks -= weeks(2);
      if (_weeks != weeks(7)) {
        return false;
      }
    }

    {
      weeks _weeks = weeks(9) + weeks(3);
      if (_weeks != weeks(12)) {
        return false;
      }
    }

    {
      days _days = days(3) + weeks(2);
      if (_days != days(17)) {
        return false;
      }
      std::cerr << "3 days + 2 weeks = " << _days << " days" << std::endl;
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
