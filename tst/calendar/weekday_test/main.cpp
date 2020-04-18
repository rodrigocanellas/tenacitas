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
      std::cerr << __LINE__ << " w = " << _w << std::endl;
      if (_stream.str() != "fri") {
        return false;
      }
    }

    {
      days _days = weekday::fri - weekday::mon;
      std::cerr << __LINE__ << " diff = " << _days << std::endl;
      if (_days != days(4)) {
        return false;
      }
    }

    {
      days _days = weekday::tue - weekday::sun;
      std::cerr << __LINE__ << " sunday is " << _days << " days away from '"
                << weekday::tue << "'" << std::endl;

      if (_days != days(2)) {
        return false;
      }
    }

    {
      days _days = weekday::sat - weekday::mon;
      std::cerr << __LINE__ << " there are " << _days << " days from '"
                << weekday::mon << "' until saturday" << std::endl;

      if (_days != days(5)) {
        return false;
      }
    }

    {
      weekday _wek(weekday::wed);
      ++_wek;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::thu) {
        return false;
      }
    }

    {
      weekday _wek(weekday::mon);
      _wek++;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::tue) {
        return false;
      }
    }

    {
      weekday _wek(weekday::sat);
      ++_wek;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::sun) {
        return false;
      }
    }

    {
      weekday _wek(weekday::wed);
      --_wek;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::tue) {
        return false;
      }
    }

    {
      weekday _wek(weekday::mon);
      _wek--;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::sun) {
        return false;
      }
    }

    {
      weekday _wek(weekday::sun);
      _wek--;
      std::cerr << __LINE__ << " new weekday = " << _wek << std::endl;
      if (_wek != weekday::sat) {
        return false;
      }
    }

    {
      if (!(weekday::wed > weekday::mon)) {
        std::cerr << __LINE__ << " " << weekday::wed << " is not greater than "
                  << weekday::mon << std::endl;
        return false;
      }
    }
    return true;
  }

  static std::string desc() { return "Basic 'weekday' tests"; }

  static std::string name() { return "weekday_test"; }
};

int main(int argc, char **argv) { tester::test::run<weekday_test>(argc, argv); }
