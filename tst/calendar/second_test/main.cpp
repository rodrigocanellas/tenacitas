
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <calendar/gregorian/second.h>
#include <tester/test.h>

using namespace tenacitas;

struct second_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      std::stringstream _stream;
      _stream << 3_sec;
      std::cerr << "seconds = " << _stream.str() << std::endl;
      if (_stream.str() != "3") {
        return false;
      }
    }

    {
      std::stringstream _stream;
      second _sec = 18_sec;
      _stream << _sec;
      std::cerr << "seconds = " << _stream.str() << std::endl;
      if (_stream.str() != "18") {
        return false;
      }
    }

    {
      if (1_sec >= 2_sec) {
        return false;
      }
    }

    {
      if (1_sec == 2_sec) {
        return false;
      }
    }

    {
      if (10_sec <= 6_sec) {
        return false;
      }
    }

    {
      try {
        second _s = 62_sec;
        std::cerr << "62 secs? " << _s << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      uint16_t _value = 21;
      second _sec(_value);
      if (_sec != 21_sec) {
        return false;
      }
      if (_sec != second::_21) {
        return false;
      }
      std::cerr << "second from variable = " << _sec << std::endl;
    }

    {
      try {
        uint16_t _value = 93;
        second _sec(_value);
        std::cerr << "93 seconds from variable? = " << _sec << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    return true;
  }

  static constexpr const char *name() { return "second_test"; }
  static constexpr const char * desc() { return "Basic test for 'second' class"; };
};

int main(int argc, char **argv) {
  tester::test _test(argc, argv);

  _test.run<second_test>();
}
