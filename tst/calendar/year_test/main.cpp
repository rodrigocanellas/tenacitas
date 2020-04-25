
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <calendar/gregorian/year.h>
#include <tester/test.h>

using namespace tenacitas;

struct year_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;
    {
      year _year(2006);
      std::stringstream _stream;
      _stream << _year;
      if (_stream.str() != "2006") {
        return false;
      }
      std::cerr << "year 2006 = " << _year << std::endl;
    }

    {
      year _year(1966);
      if (_year != _year) {
        return false;
      }
    }

    {
      year _year(1958);
      if (_year > year(1961)) {
        std::cerr << _year << " should be '<' " << year(1961) << std::endl;
      }
    }

    {
      year _year(1966);
      if (_year <= year(1961)) {
        std::cerr << _year << " should be '>' " << year(1961) << std::endl;
      }
    }

    return true;
  }

  static std::string desc() { return "Basic test for 'year' class"; }

  static std::string name() { return "year_test"; }
};

int main(int argc, char **argv) { tester::test::run<year_test>(argc, argv); }
