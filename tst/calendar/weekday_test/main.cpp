#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <calendar/gregorian/days.h>
#include <calendar/gregorian/weekday.h>
#include <tester/test.h>

using namespace tenacitas;

struct weekday_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;
    {
      weekday _w = {weekday::fri};
      std::stringstream _stream;
      _stream << _w;
      std::cerr << "w = " << _w << std::endl;
      if (_stream.str() != "fri") {
        return false;
      }
    }
    {
      days _days = weekday::fri - weekday::mon;
      std::cerr << "diff = " << _days << std::endl;
      if (_days != days(4)) {
        return false;
      }
    }
    {
      days _days = from_sunday(weekday::tue);
      std::cerr << "sunday is " << _days << " days away from '" << weekday::tue
                << "'" << std::endl;

      if (_days != days(2)) {
        return false;
      }
    }

    {
      days _days = until_saturday(weekday::mon);
      std::cerr << "there are " << _days << " days from '" << weekday::mon
                << "' until saturday" << std::endl;

      if (_days != days(5)) {
        return false;
      }
    }
    return true;
  }

  static std::string desc() { return "Basic 'weekday' tests"; }

  static std::string name() { return "weekday_test"; }
};

int main(int argc, char **argv) { tester::test::run<weekday_test>(argc, argv); }
