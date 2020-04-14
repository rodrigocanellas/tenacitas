
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <calendar/gregorian/second.h>
#include <tester/test.h>

using namespace tenacitas;

struct basic_second_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      std::stringstream _stream;
      _stream << second::s03;
      std::cerr << "stream = " << _stream.str() << std::endl;
      if (_stream.str() != "3") {
        return false;
      }
    }
    {
      std::stringstream _stream;
      second _sec = second::create(18);
      _stream << _sec;
      std::cerr << "stream = " << _stream.str() << std::endl;
      if (_stream.str() != "18") {
        return false;
      }
    }
    {
      if (second::s01 >= second::s02) {
        return false;
      }
    }
    {
      if (second::s01 == second::s02) {
        return false;
      }
    }
    return true;
  }

  static std::string desc() { return "Basic test for 'second' class"; }

  static std::string name() { return "basic_second_test"; }
};

int main(int argc, char **argv) {
  tester::test::run<basic_second_test>(argc, argv);
}
