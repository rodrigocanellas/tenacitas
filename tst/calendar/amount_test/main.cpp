
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include <calendar/gregorian/amounts.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/year.h>
#include <tester/test.h>

using namespace tenacitas::calendar::gregorian;
using namespace std;

struct seconds_test {
  bool operator()() {

    {
      minutes _minutes(seconds(1));

      if (_minutes != minutes(1 / static_cast<double>(60))) {
        cerr << "minutes should be " << 1 / static_cast<double>(60)
             << ", but it " << _minutes << endl;
        return false;
      }
      cerr << "1 sec = " << _minutes << " min" << endl;
    }

    {
      hours _hours(seconds(1));

      if (_hours != hours(1 / static_cast<double>(60 * 60))) {
        cerr << "hours should be " << 1 / static_cast<double>(60 * 60)
             << ", but it is " << _hours << endl;
        return false;
      }
      cerr << "1 sec = " << _hours << " hs" << endl;
    }

    {
      days _days(seconds(1));

      if (_days != days(1 / static_cast<double>(60 * 60 * 24))) {
        cerr << "days should be " << 1 / static_cast<double>(60 * 60 * 24)
             << ", but it is " << _days << endl;
        return false;
      }
      cerr << "1 sec = " << _days << " days" << endl;
    }

    {
      weeks _weeks(seconds(1));

      if (_weeks != weeks(1 / static_cast<double>(60 * 60 * 24 * 7))) {
        cerr << setprecision(20) << "weeks should be "
             << 1 / static_cast<double>(60 * 60 * 24 * 7) << ", but it is "
             << _weeks << endl;
        return false;
      }
      cerr << "1 sec = " << _weeks << " weeks" << endl;
    }

    {
      months _months(seconds(1));

      if (_months != months(1 / static_cast<double>(60 * 60 * 24 * 365 / 12))) {
        cerr << setprecision(20) << "months should be "
             << 1 / static_cast<double>(60 * 60 * 24 * 365 / 12)
             << ", but it is " << _months << endl;
        return false;
      }
      cerr << "1 sec = " << _months << " months" << endl;
    }

    {
      years _years(seconds(1));

      if (_years != years(1 / static_cast<double>(60 * 60 * 24 * 365))) {
        cerr << setprecision(20) << "years should be "
             << 1 / static_cast<double>(60 * 60 * 24 * 365) << ", but it is "
             << _years << endl;
        return false;
      }
      cerr << "1 sec = " << _years << " years" << endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'seconds'";
  }

  static constexpr const char *name() { return "seconds_test"; }
};

struct minutes_test {
  bool operator()() {

    {
      hours _hours(minutes(1));
      if (_hours != hours(1 / static_cast<double>(60))) {
        cerr << "hours should be " << 1 / static_cast<double>(60)
             << ", but it is " << _hours << endl;
        return false;
      }

      cerr << "1 min = " << _hours << " hs" << endl;
    }

    {
      days _days(minutes(1));
      if (_days != days(1 / static_cast<double>(60 * 24))) {
        cerr << "days should be " << 1 / static_cast<double>(60 * 24)
             << ", but it is " << _days << endl;
        return false;
      }

      cerr << "1 min = " << _days << " days" << endl;
    }

    {
      weeks _weeks(minutes(1));
      if (_weeks != weeks(1 / static_cast<double>(60 * 24 * 7))) {
        cerr << "weeks should be " << 1 / static_cast<double>(60 * 24 * 7)
             << ", but it is " << _weeks << endl;
        return false;
      }

      cerr << "1 min = " << _weeks << " weeks" << endl;
    }

    {
      months _months(minutes(1));
      if (_months != months(1 / static_cast<double>(60 * 24 * 365 / 12))) {
        cerr << "months should be "
             << 1 / static_cast<double>(60 * 24 * 365 / 12) << ", but it is "
             << _months << endl;
        return false;
      }

      cerr << "1 min = " << _months << " months" << endl;
    }

    {
      years _years(minutes(1));
      if (_years != years(1 / static_cast<double>(60 * 24 * 365))) {
        cerr << "years should be " << 1 / static_cast<double>(60 * 24 * 365)
             << ", but it is " << _years << endl;
        return false;
      }

      cerr << " 1min = " << _years << " years" << endl;
    }

    {
      seconds _seconds(minutes(1));
      if (_seconds != seconds(60)) {
        cerr << "seconds should be " << 60 << ", but it is " << _seconds
             << endl;

        return false;
      }
      cerr << "1 min = " << _seconds << " secs" << endl;
    }

    {
      seconds _seconds = minutes(4);
      if (_seconds != seconds(240)) {
        cerr << "seconds should be " << 240 << ", but it is " << _seconds
             << endl;
        return false;
      }
      cerr << "4 min = " << _seconds << " secs" << endl;
    }

    {
      seconds _seconds = minutes(1.5);
      if (_seconds != seconds(90)) {
        cerr << "seconds should be " << 90 << ", but it is " << _seconds
             << endl;
        return false;
      }
      cerr << "1.5 min = " << _seconds << " secs" << endl;
    }

    {
      seconds _seconds = minutes(-7);
      if (_seconds != seconds(-420)) {
        cerr << "seconds should be " << -420 << ", but it is " << _seconds
             << endl;
        return false;
      }
      cerr << "-7 min = " << _seconds << "secs" << endl;
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
      days _days(hours(1));
      if (_days != days(1 / static_cast<double>(24))) {
        cerr << "days should be " << 1 / static_cast<double>(24)
             << ", but it is " << _days << endl;
        return false;
      }

      cerr << "1 hour = " << _days << " days" << endl;
    }

    {
      weeks _weeks(hours(1));
      if (_weeks != weeks(1 / static_cast<double>(24 * 7))) {
        cerr << "weeks should be " << 1 / static_cast<double>(24 * 7)
             << ", but it is " << _weeks << endl;
        return false;
      }

      cerr << "1 hour = " << _weeks << " weeks" << endl;
    }

    {
      months _months(hours(1));
      if (_months != months(1 / static_cast<double>(24 * 365 / 12))) {
        cerr << "months should be " << 1 / static_cast<double>(24 * 365 / 12)
             << ", but it is " << _months << endl;
        return false;
      }

      cerr << "1 hour = " << _months << " months" << endl;
    }

    {
      years _years(hours(1));
      if (_years != years(1 / static_cast<double>(24 * 365))) {
        cerr << "years should be " << 1 / static_cast<double>(24 * 365)
             << ", but it is " << _years << endl;
        return false;
      }

      cerr << "1 hour = " << _years << " years" << endl;
    }

    {
      seconds _seconds(hours(1));
      if (_seconds != seconds(60 * 60)) {
        cerr << "seconds should be " << 60 * 60 << ", but it is " << _seconds
             << endl;

        return false;
      }
      cerr << "1 hour = " << _seconds << " secs" << endl;
    }

    {
      minutes _minutes(hours(1));
      if (_minutes != minutes(60)) {
        cerr << "minutes should be " << 60 << ", but it is " << _minutes
             << endl;

        return false;
      }
      cerr << "1 hours = " << _minutes << " min" << endl;
    }

    {
      minutes _minutes = hours(4);
      if (_minutes != minutes(240)) {
        cerr << "minutes should be " << 240 << ", but it is " << _minutes
             << endl;
        return false;
      }
      cerr << "4 hours = " << _minutes << " min" << endl;
    }

    {
      minutes _minutes = hours(1.5);
      if (_minutes != minutes(90)) {
        cerr << "minutes should be " << 90 << ", but it is " << _minutes
             << endl;
        return false;
      }
      cerr << "1.5 hous = " << _minutes << " min" << endl;
    }

    {
      minutes _minutes = hours(-7);
      if (_minutes != minutes(-420)) {
        cerr << "minutes should be " << -420 << ", but it is " << _minutes
             << endl;
        return false;
      }
      cerr << "-7 hours = " << _minutes << " min" << endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'hours'";
  }

  static constexpr const char *name() { return "hours_test"; }
};

// struct days_test {
//  bool operator()() {

//    {
//      hours _hours = convert<days, hours>();
//      if (_hours != hours(24)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<days, hours>(days(4));
//      if (_hours != hours(96)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<days, hours>(days(1.5));
//      if (_hours != hours(36)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<days, hours>(days(-3));
//      if (_hours != hours(-72)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<days, minutes>();
//      if (_minutes != minutes(1440)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<days, minutes>(days(4));
//      if (_minutes != minutes(5760)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<days, minutes>(days(1.5));
//      if (_minutes != minutes(2160)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<days, minutes>(days(-3));
//      if (_minutes != minutes(-4320)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<days, seconds>();
//      if (_seconds != seconds(86400)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<days, seconds>(days(4));
//      if (_seconds != seconds(345600)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<days, seconds>(days(1.5));
//      if (_seconds != seconds(129600)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<days, seconds>(days(-3));
//      if (_seconds != seconds(-259200)) {
//        return false;
//      }
//    }

//    return true;
//  }

//  static constexpr const char *desc() {
//    return "Tests conversions from 'days'";
//  }

//  static constexpr const char *name() { return "days_test"; }
//};

// struct weeks_test {
//  bool operator()() {

//    {
//      days _days = convert<weeks, days>();
//      if (_days != days(7)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<weeks, days>(weeks(4));
//      if (_days != days(28)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<weeks, days>(weeks(1.5));
//      std::cerr << "How many days in 1.5 weeks? " << _days << std::endl;
//      if (_days != days(10.5)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<weeks, days>(weeks(-3));
//      if (_days != days(-21)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<weeks, hours>();
//      if (_hours != hours(168)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<weeks, hours>(weeks(4));
//      if (_hours != hours(672)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<weeks, hours>(weeks(1.5));
//      std::cerr << "How many hours in 1.5 weeks? " << _hours << std::endl;
//      if (_hours != hours(252)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<weeks, hours>(weeks(-3));
//      if (_hours != hours(-504)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<weeks, minutes>();
//      if (_minutes != minutes(10080)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<weeks, minutes>(weeks(4));
//      if (_minutes != minutes(40320)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<weeks, minutes>(weeks(1.5));
//      std::cerr << "How many minutes in 1.5 weeks? " << _minutes << std::endl;
//      if (_minutes != minutes(15120)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<weeks, minutes>(weeks(-3));
//      if (_minutes != minutes(-30240)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<weeks, seconds>();
//      if (_seconds != seconds(604800)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<weeks, seconds>(weeks(4));
//      if (_seconds != seconds(2419200)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<weeks, seconds>(weeks(1.5));
//      std::cerr << "How many seconds in 1.5 weeks? " << _seconds << std::endl;
//      if (_seconds != seconds(907200)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<weeks, seconds>(weeks(-3));
//      if (_seconds != seconds(-1814400)) {
//        return false;
//      }
//    }

//    return true;
//  }

//  static constexpr const char *desc() {
//    return "Tests conversions from 'weeks'";
//  }

//  static constexpr const char *name() { return "weeks_test"; }
//};

// struct months_test {

//  bool operator()() {

//    {
//      days _days = convert<months, days>();
//      if (_days != days(30)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<months, days>(months(4));
//      if (_days != days(120)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<months, days>(months(1.5));
//      std::cerr << "How many days in 1.5 months? " << _days << std::endl;
//      if (_days != days(45)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<months, days>(months(-3));
//      if (_days != days(-90)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<months, hours>();
//      if (_hours != hours(720)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<months, hours>(months(4));
//      if (_hours != hours(2880)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<months, hours>(months(1.5));
//      std::cerr << "How many hours in 1.5 months? " << _hours << std::endl;
//      if (_hours != hours(1080)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<months, hours>(months(-3));
//      if (_hours != hours(-2160)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<months, minutes>();
//      if (_minutes != minutes(43200)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<months, minutes>(months(4));
//      if (_minutes != minutes(172800)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<months, minutes>(months(1.5));
//      std::cerr << "How many minutes in 1.5 months? " << _minutes <<
//      std::endl; if (_minutes != minutes(64800)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<months, minutes>(months(-3));
//      if (_minutes != minutes(-129600)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<months, seconds>();
//      if (_seconds != seconds(2592000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<months, seconds>(months(4));
//      if (_seconds != seconds(10368000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<months, seconds>(months(1.5));
//      std::cerr << "How many seconds in 1.5 months? " << _seconds <<
//      std::endl; if (_seconds != seconds(3888000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<months, seconds>(months(-3));
//      if (_seconds != seconds(-7776000)) {
//        return false;
//      }
//    }

//    return true;
//  }

//  static constexpr const char *desc() {
//    return "Tests conversions from 'months'";
//  }

//  static constexpr const char *name() { return "months_test"; }
//};

// struct years_test {

//  bool operator()() {

//    {
//      months _months = convert<years, months>();
//      if (_months != months(12)) {
//        return false;
//      }
//    }

//    {
//      months _months = convert<years, months>(years(4));
//      if (_months != months(48)) {
//        return false;
//      }
//    }

//    {
//      months _months = convert<years, months>(years(1.5));
//      std::cerr << "How many months in 1.5 years? " << _months << std::endl;
//      if (_months != months(18)) {
//        return false;
//      }
//    }

//    {
//      months _months = convert<years, months>(years(-3));
//      if (_months != months(-36)) {
//        return false;
//      }
//    }

//    {
//      weeks _weeks = convert<years, weeks>();
//      if (_weeks != weeks(52)) {
//        return false;
//      }
//    }

//    {
//      weeks _weeks = convert<years, weeks>(years(4));
//      if (_weeks != weeks(208)) {
//        return false;
//      }
//    }

//    {
//      weeks _weeks = convert<years, weeks>(years(1.5));
//      std::cerr << "How many weeks in 1.5 years? " << _weeks << std::endl;
//      if (_weeks != weeks(78)) {
//        return false;
//      }
//    }

//    {
//      weeks _weeks = convert<years, weeks>(years(-3));
//      if (_weeks != weeks(-156)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<years, days>();
//      if (_days != days(365)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<years, days>(years(4));
//      if (_days != days(1460)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<years, days>(years(1.5));
//      std::cerr << "How many days in 1.5 years? " << _days << std::endl;
//      if (_days != days(547.5)) {
//        return false;
//      }
//    }

//    {
//      days _days = convert<years, days>(years(-3));
//      if (_days != days(-1095)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<years, hours>();
//      if (_hours != hours(8760)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<years, hours>(years(4));
//      if (_hours != hours(35040)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<years, hours>(years(1.5));
//      std::cerr << "How many hours in 1.5 years? " << _hours << std::endl;
//      if (_hours != hours(13140)) {
//        return false;
//      }
//    }

//    {
//      hours _hours = convert<years, hours>(years(-3));
//      if (_hours != hours(-26280)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<years, minutes>();
//      if (_minutes != minutes(525600)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<years, minutes>(years(4));
//      if (_minutes != minutes(2102400)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<years, minutes>(years(1.5));
//      std::cerr << "How many minutes in 1.5 years? " << _minutes << std::endl;
//      if (_minutes != minutes(788400)) {
//        return false;
//      }
//    }

//    {
//      minutes _minutes = convert<years, minutes>(years(-3));
//      if (_minutes != minutes(-1576800)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<years, seconds>();
//      if (_seconds != seconds(31536000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<years, seconds>(years(4));
//      if (_seconds != seconds(126144000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<years, seconds>(years(1.5));
//      std::cerr << "How many seconds in 1.5 years? " << _seconds << std::endl;
//      if (_seconds != seconds(47304000)) {
//        return false;
//      }
//    }

//    {
//      seconds _seconds = convert<years, seconds>(years(-3));
//      if (_seconds != seconds(-94608000)) {
//        return false;
//      }
//    }

//    return true;
//  }

//  static constexpr const char *desc() {
//    return "Tests conversions from 'years'";
//  }

//  static constexpr const char *name() { return "years_test"; }
//};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<seconds_test>();
  _test.run<minutes_test>();
  _test.run<hours_test>();
  //  _test.run<days_test>();
  //  _test.run<weeks_test>();
  //  _test.run<months_test>();
  //  _test.run<years_test>();
}
