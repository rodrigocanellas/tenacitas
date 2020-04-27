
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/weeks.h>
#include <tester/test.h>

using namespace tenacitas::calendar::gregorian;

struct mins2secs {
  bool operator()() {

    {
      seconds _secs = convert<minutes, seconds>();
      if (_secs != seconds(60)) {
        return false;
      }
    }

    {
      seconds _secs = convert<minutes, seconds>(minutes(5));
      if (_secs != seconds(300)) {
        return false;
      }
    }

    return true;
  }

  static std::string desc() {
    return "Tests conversions from 'minutes' to 'seconds'";
  }

  static std::string name() { return "mins2secs"; }
};

struct amount_basic_test {
  bool operator()() {

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
  tenacitas::tester::test _test(argc, argv);
  _test.run<amount_basic_test>();
  _test.run<mins2secs>();
}
