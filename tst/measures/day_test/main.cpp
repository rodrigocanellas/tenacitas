
#include <cstdint>
#include <iostream>
#include <string>

#include <measures/day.h>
#include <tester/test.h>

using namespace tenacitas;

struct day_test {
  bool operator()() {
    using namespace tenacitas::measures;

    {
      std::stringstream _stream;
      _stream << day::_03;
      std::cerr << "days = " << _stream.str() << std::endl;
      if (_stream.str() != "3") {
        return false;
      }
    }

    {
      std::stringstream _stream;
      day _day = 18_day;
      _stream << _day;
      std::cerr << "days = " << _stream.str() << std::endl;
      if (_stream.str() != "18") {
        return false;
      }
    }

    {
      if (day::_01 >= day::_02) {
        return false;
      }
    }

    {
      if (day::_01 == day::_02) {
        return false;
      }
    }

    {
      if (10_day <= 6_day) {
        return false;
      }
    }

    {
      try {
        day _day = 32_day;
        std::cerr << "32 days? " << _day << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      uint16_t _value = 21;
      day _day(_value);
      if (_day != 21_day) {
        return false;
      }
      if (_day != day::_21) {
        return false;
      }
      std::cerr << "day from variable = " << _day << std::endl;
    }

    {
      try {
        uint16_t _value = 93;
        day _day(_value);
        std::cerr << "93 days from variable? = " << _day << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }
    return true;
  }
  static constexpr const char *desc() { return "Basic test for 'day' class"; }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, day_test);
}
