
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>
#include <tester/test.h>

using namespace tenacitas;

struct amount_default_ctor {
  bool operator()() {
    calendar::gregorian::days _days;

    std::stringstream _stream;

    _stream << _days;

    std::cerr << "days = " << _days << std::endl;

    return (_stream.str() == "0");
  }

  static std::string desc() { return "Test default constructor for 'days'"; }

  static std::string name() { return "amount_default_ctor"; }
};

int main(int argc, char **argv) {
  tester::test::run<amount_default_ctor>(argc, argv);
}
