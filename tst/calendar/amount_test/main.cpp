
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/months.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/weeks.h>
#include <calendar/gregorian/years.h>
#include <tester/test.h>

using namespace tenacitas::calendar::gregorian;

struct minutes_test {
  bool operator()() {

    {
      seconds _secs = convert<minutes, seconds>();
      if (_secs != seconds(60)) {
        return false;
      }
    }

    {
      seconds _secs = convert<minutes, seconds>(minutes(5));
      if (_secs != seconds(300)) {
        return false;
      }
    }

    {
      seconds _secs = convert<minutes, seconds>(minutes(2.5));
      if (_secs != seconds(150)) {
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'minutes'";
  }

  static constexpr const char *name() { return "minutes_test"; }
};

struct hours_test {
  bool operator()() {
    {
      minutes _minutes = convert<hours, minutes>();
      if (_minutes != minutes(60)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<hours, minutes>(hours(4));
      if (_minutes != minutes(240)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<hours, minutes>(hours(1.5));
      if (_minutes != minutes(90)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<hours, seconds>();
      if (_seconds != seconds(3600)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<hours, seconds>(hours(4));
      if (_seconds != seconds(14400)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<hours, seconds>(hours(1.5));
      if (_seconds != seconds(5400)) {
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'hours'";
  }

  static constexpr const char *name() { return "hours_test"; }
};

struct days_test {
  bool operator()() {

    {
      hours _hours = convert<days, hours>();
      if (_hours != hours(24)) {
        return false;
      }
    }

    {
      hours _hours = convert<days, hours>(days(4));
      if (_hours != hours(96)) {
        return false;
      }
    }

    {
      hours _hours = convert<days, hours>(days(1.5));
      if (_hours != hours(36)) {
        return false;
      }
    }

    {
      hours _hours = convert<days, hours>(days(-3));
      if (_hours != hours(-72)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<days, minutes>();
      if (_minutes != minutes(1440)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<days, minutes>(days(4));
      if (_minutes != minutes(5760)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<days, minutes>(days(1.5));
      if (_minutes != minutes(2160)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<days, minutes>(days(-3));
      if (_minutes != minutes(-4320)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<days, seconds>();
      if (_seconds != seconds(86400)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<days, seconds>(days(4));
      if (_seconds != seconds(345600)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<days, seconds>(days(1.5));
      if (_seconds != seconds(129600)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<days, seconds>(days(-3));
      if (_seconds != seconds(-259200)) {
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'days'";
  }

  static constexpr const char *name() { return "days_test"; }
};

struct weeks_test {
  bool operator()() {

    {
      days _days = convert<weeks, days>();
      if (_days != days(7)) {
        return false;
      }
    }

    {
      days _days = convert<weeks, days>(weeks(4));
      if (_days != days(28)) {
        return false;
      }
    }

    {
      days _days = convert<weeks, days>(weeks(1.5));
      std::cerr << "How many days in 1.5 weeks? " << _days << std::endl;
      if (_days != days(10.5)) {
        return false;
      }
    }

    {
      days _days = convert<weeks, days>(weeks(-3));
      if (_days != days(-21)) {
        return false;
      }
    }

    {
      hours _hours = convert<weeks, hours>();
      if (_hours != hours(168)) {
        return false;
      }
    }

    {
      hours _hours = convert<weeks, hours>(weeks(4));
      if (_hours != hours(672)) {
        return false;
      }
    }

    {
      hours _hours = convert<weeks, hours>(weeks(1.5));
      std::cerr << "How many hours in 1.5 weeks? " << _hours << std::endl;
      if (_hours != hours(252)) {
        return false;
      }
    }

    {
      hours _hours = convert<weeks, hours>(weeks(-3));
      if (_hours != hours(-504)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<weeks, minutes>();
      if (_minutes != minutes(10080)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<weeks, minutes>(weeks(4));
      if (_minutes != minutes(40320)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<weeks, minutes>(weeks(1.5));
      std::cerr << "How many minutes in 1.5 weeks? " << _minutes << std::endl;
      if (_minutes != minutes(15120)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<weeks, minutes>(weeks(-3));
      if (_minutes != minutes(-30240)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<weeks, seconds>();
      if (_seconds != seconds(604800)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<weeks, seconds>(weeks(4));
      if (_seconds != seconds(2419200)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<weeks, seconds>(weeks(1.5));
      std::cerr << "How many seconds in 1.5 weeks? " << _seconds << std::endl;
      if (_seconds != seconds(907200)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<weeks, seconds>(weeks(-3));
      if (_seconds != seconds(-1814400)) {
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'weeks'";
  }

  static constexpr const char *name() { return "weeks_test"; }
};

struct months_test {

  bool operator()() {

    {
      days _days = convert<months, days>();
      if (_days != days(30)) {
        return false;
      }
    }

    {
      days _days = convert<months, days>(months(4));
      if (_days != days(120)) {
        return false;
      }
    }

    {
      days _days = convert<months, days>(months(1.5));
      std::cerr << "How many days in 1.5 months? " << _days << std::endl;
      if (_days != days(45)) {
        return false;
      }
    }

    {
      days _days = convert<months, days>(months(-3));
      if (_days != days(-90)) {
        return false;
      }
    }

    {
      hours _hours = convert<months, hours>();
      if (_hours != hours(720)) {
        return false;
      }
    }

    {
      hours _hours = convert<months, hours>(months(4));
      if (_hours != hours(2880)) {
        return false;
      }
    }

    {
      hours _hours = convert<months, hours>(months(1.5));
      std::cerr << "How many hours in 1.5 months? " << _hours << std::endl;
      if (_hours != hours(1080)) {
        return false;
      }
    }

    {
      hours _hours = convert<months, hours>(months(-3));
      if (_hours != hours(-2160)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<months, minutes>();
      if (_minutes != minutes(43200)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<months, minutes>(months(4));
      if (_minutes != minutes(172800)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<months, minutes>(months(1.5));
      std::cerr << "How many minutes in 1.5 months? " << _minutes << std::endl;
      if (_minutes != minutes(64800)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<months, minutes>(months(-3));
      if (_minutes != minutes(-129600)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<months, seconds>();
      if (_seconds != seconds(2592000)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<months, seconds>(months(4));
      if (_seconds != seconds(10368000)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<months, seconds>(months(1.5));
      std::cerr << "How many seconds in 1.5 months? " << _seconds << std::endl;
      if (_seconds != seconds(3888000)) {
        return false;
      }
    }

    {
      seconds _seconds = convert<months, seconds>(months(-3));
      if (_seconds != seconds(-7776000)) {
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'months'";
  }

  static constexpr const char *name() { return "months_test"; }
};

struct years_test {

  bool operator()() {

    {
      months _months = convert<years, months>();
      if (_months != months(12)) {
        return false;
      }
    }

    {
      months _months = convert<years, months>(years(4));
      if (_months != months(48)) {
        return false;
      }
    }

    {
      months _months = convert<years, months>(years(1.5));
      std::cerr << "How many months in 1.5 years? " << _months << std::endl;
      if (_months != months(18)) {
        return false;
      }
    }

    {
      months _months = convert<years, months>(years(-3));
      if (_months != months(-36)) {
        return false;
      }
    }

    {
      weeks _weeks = convert<years, weeks>();
      if (_weeks != weeks(52)) {
        return false;
      }
    }

    {
      weeks _weeks = convert<years, weeks>(years(4));
      if (_weeks != weeks(208)) {
        return false;
      }
    }

    {
      weeks _weeks = convert<years, weeks>(years(1.5));
      std::cerr << "How many weeks in 1.5 years? " << _weeks << std::endl;
      if (_weeks != weeks(78)) {
        return false;
      }
    }

    {
      weeks _weeks = convert<years, weeks>(years(-3));
      if (_weeks != weeks(-156)) {
        return false;
      }
    }

    {
      days _days = convert<years, days>();
      if (_days != days(365)) {
        return false;
      }
    }

    {
      days _days = convert<years, days>(years(4));
      if (_days != days(1460)) {
        return false;
      }
    }

    {
      days _days = convert<years, days>(years(1.5));
      std::cerr << "How many days in 1.5 years? " << _days << std::endl;
      if (_days != days(547.5)) {
        return false;
      }
    }

    {
      days _days = convert<years, days>(years(-3));
      if (_days != days(-1095)) {
        return false;
      }
    }

    {
      hours _hours = convert<years, hours>();
      if (_hours != hours(8760)) {
        return false;
      }
    }

    {
      hours _hours = convert<years, hours>(years(4));
      if (_hours != hours(35040)) {
        return false;
      }
    }

    {
      hours _hours = convert<years, hours>(years(1.5));
      std::cerr << "How many hours in 1.5 years? " << _hours << std::endl;
      if (_hours != hours(13140)) {
        return false;
      }
    }

    {
      hours _hours = convert<years, hours>(years(-3));
      if (_hours != hours(-26280)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<years, minutes>();
      if (_minutes != minutes(525600)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<years, minutes>(years(4));
      if (_minutes != minutes(2102400)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<years, minutes>(years(1.5));
      std::cerr << "How many minutes in 1.5 years? " << _minutes << std::endl;
      if (_minutes != minutes(788400)) {
        return false;
      }
    }

    {
      minutes _minutes = convert<years, minutes>(years(-3));
      if (_minutes != minutes(-1576800)) {
        return false;
      }
    }

    //    {
    //      seconds _seconds = convert<years, seconds>();
    //      if (_seconds != seconds(2592000)) {
    //        return false;
    //      }
    //    }

    //    {
    //      seconds _seconds = convert<years, seconds>(years(4));
    //      if (_seconds != seconds(10368000)) {
    //        return false;
    //      }
    //    }

    //    {
    //      seconds _seconds = convert<years, seconds>(years(1.5));
    //      std::cerr << "How many seconds in 1.5 years? " << _seconds <<
    //      std::endl; if (_seconds != seconds(3888000)) {
    //        return false;
    //      }
    //    }

    //    {
    //      seconds _seconds = convert<years, seconds>(years(-3));
    //      if (_seconds != seconds(-7776000)) {
    //        return false;
    //      }
    //    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'years'";
  }

  static constexpr const char *name() { return "years_test"; }
};

struct amount_test {
  bool operator()() {

    {
      weeks _weeks;
      if (_weeks != weeks(0)) {
        return false;
      }
    }

    {
      weeks _weeks(8);
      if (_weeks != weeks(8)) {
        return false;
      }
    }

    {
      weeks _weeks(5);
      _weeks += weeks(8);
      if (_weeks != weeks(13)) {
        return false;
      }
    }

    {
      weeks _weeks(9);
      _weeks -= weeks(2);
      if (_weeks != weeks(7)) {
        return false;
      }
    }

    {
      weeks _weeks = weeks(9) + weeks(3);
      if (_weeks != weeks(12)) {
        return false;
      }
    }

    {
      days _days = days(3) + weeks(2);
      if (_days != days(17)) {
        return false;
      }
      std::cerr << "3 days + 2 weeks = " << _days << " days" << std::endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Executes basic tests on 'weeks', a type of 'amount'";
  }

  static constexpr const char *name() { return "amount_test"; }
};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<amount_test>();
  _test.run<minutes_test>();
  _test.run<hours_test>();
  _test.run<days_test>();
  _test.run<weeks_test>();
  _test.run<months_test>();
  _test.run<years_test>();
}
