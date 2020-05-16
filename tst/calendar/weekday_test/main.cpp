#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

//#include <calendar/gregorian/days.h>
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
      if (_stream.str() != "fri") {
        return false;
      }
      std::cerr << " w = " << _w << std::endl;
    }

    //    {
    //      days _days = weekday::fri - weekday::mon;
    //      std::cerr  << " diff = " << _days << std::endl;
    //      if (_days != days(4)) {
    //        return false;
    //      }
    //    }

    //    {
    //      days _days = weekday::tue - weekday::sun;
    //      std::cerr  << " sunday is " << _days << " days away from
    //      '"
    //                << weekday::tue << "'" << std::endl;

    //      if (_days != days(2)) {
    //        return false;
    //      }
    //    }

    //    {
    //      days _days = weekday::sat - weekday::mon;
    //      std::cerr  << " there are " << _days << " days from '"
    //                << weekday::mon << "' until saturday" << std::endl;

    //      if (_days != days(5)) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::wed);
    //      ++_wek;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::thu) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::mon);
    //      _wek++;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::tue) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::sat);
    //      ++_wek;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::sun) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::wed);
    //      --_wek;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::tue) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::mon);
    //      _wek--;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::sun) {
    //        return false;
    //      }
    //    }

    //    {
    //      weekday _wek(weekday::sun);
    //      _wek--;
    //      std::cerr  << " new weekday = " << _wek << std::endl;
    //      if (_wek != weekday::sat) {
    //        return false;
    //      }
    //    }

    {
      if (!(weekday::wed > weekday::mon)) {
        std::cerr << " " << weekday::wed << " is not greater than "
                  << weekday::mon << std::endl;
        return false;
      }
    }

    {
      try {
        weekday _weekday(10);
        std::cerr << "weekday 10? " << _weekday << std::endl;
        return false;
      } catch (const std::exception &_ex) {
        std::cerr << _ex.what() << std::endl;
      }
    }

    return true;
  }

  static constexpr const char *name() { return "weekday_test"; }
  static constexpr const char *desc() {
    return "Basic test for 'weekday' class";
  }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, weekday_test);
}
