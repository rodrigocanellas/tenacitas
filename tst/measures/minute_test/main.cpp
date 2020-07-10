
#include <cstdint>
#include <iostream>
#include <string>

#include <measures/minute.h>
#include <tester/test.h>

using namespace tenacitas;

struct minute_test {
  bool operator()() {
    using namespace tenacitas::measures;

    {
      std::stringstream _stream;
      _stream << minute::_03;
      std::cerr << "minutes = " << _stream.str() << std::endl;
      if (_stream.str() != "3") {
        return false;
      }
    }

    {
      std::stringstream _stream;
      minute _minute = 18_min;
      _stream << _minute;
      std::cerr << "minutes = " << _stream.str() << std::endl;
      if (_stream.str() != "18") {
        return false;
      }
    }

    {
      if (minute::_01 >= minute::_02) {
        return false;
      }
    }

    {
      if (minute::_01 == minute::_02) {
        return false;
      }
    }

    {
      if (10_min <= 6_min) {
        return false;
      }
    }

    {
      try {
        minute _minute = 62_min;
        std::cerr << "62 minutes? " << _minute << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      uint16_t _value = 21;
      minute _minute(_value);
      if (_minute != 21_min) {
        return false;
      }
      if (_minute != minute::_21) {
        return false;
      }
      std::cerr << "second from variable = " << _minute << std::endl;
    }

    {
      try {
        uint16_t _value = 93;
        minute _min(_value);
        std::cerr << "93 minutes from variable? = " << _min << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Basic test for 'minute' class";
  }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, minute_test);
}
