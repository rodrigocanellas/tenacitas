
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/seconds.h>
#include <tester/test.h>

using namespace tenacitas;

struct amounts_conversions {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      constexpr minutes _minutes(1);
      constexpr seconds _seconds = convert<minutes, seconds>(_minutes);
      std::cerr << _minutes << " minute to seconds = " << _seconds << std::endl;
      if (_seconds != seconds(60)) {
        return false;
      }
    }
    return true;
  }

  static std::string desc() { return "Basic amount conversions tests"; }

  static std::string name() { return "amounts_conversions"; }
};

int main(int argc, char **argv) {
  tester::test::run<amounts_conversions>(argc, argv);
}
