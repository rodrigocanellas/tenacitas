
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <calendar/gregorian/month.h>
#include <tester/test.h>

using namespace tenacitas;

struct month_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      std::stringstream _stream;
      _stream << month::jan;
      if (_stream.str() != "jan") {
        return false;
      }

      std::cerr << "month::jan = " << month::jan << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::feb;
      if (_stream.str() != "feb") {
        return false;
      }

      std::cerr << "month::feb = " << month::feb << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::mar;
      if (_stream.str() != "mar") {
        return false;
      }

      std::cerr << "month::mar = " << month::mar << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::apr;
      if (_stream.str() != "apr") {
        return false;
      }

      std::cerr << "month::apr = " << month::apr << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::may;
      if (_stream.str() != "may") {
        return false;
      }

      std::cerr << "month::may = " << month::may << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::jun;
      if (_stream.str() != "jun") {
        return false;
      }

      std::cerr << "month::jun = " << month::jun << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::jul;
      if (_stream.str() != "jul") {
        return false;
      }

      std::cerr << "month::jul = " << month::jul << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::ago;
      if (_stream.str() != "ago") {
        return false;
      }

      std::cerr << "month::ago = " << month::ago << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::sep;
      if (_stream.str() != "sep") {
        return false;
      }

      std::cerr << "month::sep = " << month::sep << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::oct;
      if (_stream.str() != "oct") {
        return false;
      }

      std::cerr << "month::oct = " << month::oct << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::nov;
      if (_stream.str() != "nov") {
        return false;
      }

      std::cerr << "month::nov = " << month::nov << std::endl;
    }

    {
      std::stringstream _stream;
      _stream << month::feb;
      if (_stream.str() != "feb") {
        return false;
      }

      std::cerr << "month::feb = " << month::feb << std::endl;
    }

    {
      try {
        month _month(-1);
        std::cerr << "month -1? " << _month << std::endl;
        return false;

      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      try {
        month _month(13);
        std::cerr << "month 13? " << _month << std::endl;
        return false;

      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    {
      try {
        month _month(9);
        std::cerr << "month 9 = ? " << _month << std::endl;

      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
        return false;
      }
    }

    return true;
  }
  static std::string name() { return "month_test"; }
  static std::string desc() { return "Basic test for 'month' class"; }
};

int main(int argc, char **argv) {
  tester::test _test(argc, argv);
  _test.run<month_test>();
}
