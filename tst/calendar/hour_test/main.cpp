
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/hour.h>
#include <tester/test.h>

using namespace tenacitas;

struct hour_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      std::stringstream _stream;
      _stream << hour::_03;
      std::cerr << "hours = " << _stream.str() << std::endl;
      if (_stream.str() != "3") {
        return false;
      }
    }

    {
      std::stringstream _stream;
      hour _hour = 18_hr;
      _stream << _hour;
      std::cerr << "hours = " << _stream.str() << std::endl;
      if (_stream.str() != "18") {
        return false;
      }
    }

    {
      if (hour::_01 >= hour::_02) {
        return false;
      }
    }

    {
      if (hour::_01 == hour::_02) {
        return false;
      }
    }

    {
      if (10_hr <= 6_hr) {
        return false;
      }
    }

    {
      try {
        hour _hour = 24_hr;
        std::cerr << "24 hours? " << _hour << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      uint16_t _value = 21;
      hour _hour(_value);
      if (_hour != 21_hr) {
        return false;
      }
      if (_hour != hour::_21) {
        return false;
      }
      std::cerr << "hour from variable = " << _hour << std::endl;
    }

    {
      try {
        uint16_t _value = 93;
        hour _hour(_value);
        std::cerr << "93 hours from variable? = " << _hour << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    return true;
  }

  static constexpr const char *desc() { return "Basic test for 'hour' class"; }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, hour_test);
}
