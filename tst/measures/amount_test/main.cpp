
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include <measures/day.h>
#include <measures/internal/convert_t.h>
#include <measures/month.h>
#include <measures/time_amounts.h>
#include <measures/year.h>
#include <tester/test.h>

using namespace tenacitas::measures;
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
        cerr << "weeks should be " << 1 / static_cast<double>(60 * 60 * 24 * 7)
             << ", but it is " << _weeks << endl;
        return false;
      }
      cerr << "1 sec = " << _weeks << " weeks" << endl;
    }

    {
      months _months(seconds(1));

      if (_months != months(1 / static_cast<double>(60 * 60 * 24 * 365 / 12))) {
        cerr << "months should be "
             << 1 / static_cast<double>(60 * 60 * 24 * 365 / 12)
             << ", but it is " << _months << endl;
        return false;
      }
      cerr << "1 sec = " << _months << " months" << endl;
    }

    {
      years _years(seconds(1));

      if (_years != years(1 / static_cast<double>(60 * 60 * 24 * 365))) {
        cerr << "years should be "
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

      cerr << "1min = " << _years << " years" << endl;
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
};

struct days_test {
  bool operator()() {

    {
      weeks _weeks(days(1));
      if (_weeks != weeks(1 / static_cast<double>(7))) {
        cerr << "weeks should be " << 1 / static_cast<double>(7)
             << ",  but it is " << _weeks << endl;
        return false;
      }
      cerr << "1 day = " << _weeks << " weeks" << endl;
    }

    {

      months _months(days(1));
      if (_months != months(1 / (365 / static_cast<double>(12)))) {
        cerr << "months should be " << 1 / (365 / static_cast<double>(12))
             << ",  but it is " << _months << endl;
        return false;
      }
      cerr << "1 day = " << _months << " months" << endl;
    }

    {

      years _years(days(1));
      if (_years != years(1 / static_cast<double>(365))) {
        cerr << "years should be " << 1 / static_cast<double>(365)
             << ",  but it is " << _years << endl;
        return false;
      }
      cerr << "1 day = " << _years << " years" << endl;
    }

    {
      minutes _minutes(days(1));
      if (_minutes != minutes(24 * 60)) {
        cerr << "minutes should be " << 24 * 60 << ", but it is " << _minutes
             << endl;

        return false;
      }
      cerr << "1 day = " << _minutes << " minutes" << endl;
    }

    {
      minutes _minutes = days(4);
      if (_minutes != minutes(4 * 24 * 60)) {
        cerr << "minutes should be " << 4 * 24 * 60 << ", but it is "
             << _minutes << endl;
        return false;
      }
      cerr << "4 days = " << _minutes << " minutes" << endl;
    }

    {
      minutes _minutes = days(1.5);
      if (_minutes != minutes(1.5 * static_cast<double>(24 * 60))) {
        cerr << "minutes should be " << 1.5 * 24 * 60 << ", but it is "
             << _minutes << endl;
        return false;
      }
      cerr << "1.5 days = " << _minutes << " minutes" << endl;
    }

    {
      minutes _minutes = days(-7);
      if (_minutes != minutes(-7 * 24 * 60)) {
        cerr << "minutes should be " << -7 * 24 * 60 << ", but it is "
             << _minutes << endl;
        return false;
      }
      cerr << "-7 days = " << _minutes << " minutes" << endl;
    }

    {
      seconds _seconds(days(1));
      if (_seconds != seconds(24 * 60 * 60)) {
        cerr << "seconds should be " << 24 * 60 * 60 << ", but it is "
             << _seconds << endl;

        return false;
      }
      cerr << "1 day = " << _seconds << " seconds" << endl;
    }

    {
      seconds _seconds = days(4);
      if (_seconds != seconds(4 * 24 * 60 * 60)) {
        cerr << "seconds should be " << 4 * 24 * 60 * 60 << ", but it is "
             << _seconds << endl;
        return false;
      }
      cerr << "4 days = " << _seconds << " seconds" << endl;
    }

    {
      seconds _seconds = days(1.5);
      if (_seconds != seconds(1.5 * static_cast<double>(24 * 60 * 60))) {
        cerr << "seconds should be " << 1.5 * 24 * 60 * 60 << ", but it is "
             << _seconds << endl;
        return false;
      }
      cerr << "1.5 days = " << _seconds << " seconds" << endl;
    }

    {
      seconds _seconds = days(-7);
      if (_seconds != seconds(-7 * 24 * 60 * 60)) {
        cerr << "seconds should be " << -7 * 24 * 60 * 60 << ", but it is "
             << _seconds << endl;
        return false;
      }
      cerr << "-7 days = " << _seconds << " seconds" << endl;
    }

    {
      days _days(day(18));
      if (_days != days(18)) {
        cerr << "days should be 18, but it is " << _days << " days";
        return false;
      }
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'days'";
  }
};

struct weeks_test {
  bool operator()() {

    {
      months _months(weeks(1));
      if (_months != months(1 / ((365 / static_cast<double>(12)) / 7))) {
        cerr << "months should be "
             << (1 / ((365 / static_cast<double>(12)) / 7)) << ", but it is "
             << _months << endl;
      }
      cerr << "1 weeks = " << _months << " months " << endl;
    }

    {
      years _years(weeks(1));
      if (_years != years(1 / (365 / static_cast<double>(7)))) {
        cerr << "years should be " << (1 / (365 / static_cast<double>(7)))
             << ", but it is " << _years << endl;
      }
      cerr << "1 weeks = " << _years << " years " << endl;
    }

    {
      days _days(weeks(1));
      if (_days != days(7)) {
        cerr << "days should be " << 7 << ", but it is " << _days << endl;

        return false;
      }
      cerr << "1 week = " << _days << " days" << endl;
    }

    {
      days _days = weeks(4);
      if (_days != days(4 * 7)) {
        cerr << "days should be " << 4 * 7 << ", but it is " << _days << endl;
        return false;
      }
      cerr << "4 weeks  = " << _days << " days" << endl;
    }

    {
      days _days = weeks(1.5);
      if (_days != days(1.5 * static_cast<double>(7))) {
        cerr << "days should be " << 1.5 * static_cast<double>(7)
             << ", but it is " << _days << endl;
        return false;
      }
      cerr << "1.5 weeks  = " << _days << " days" << endl;
    }

    {
      days _days = weeks(-7);
      if (_days != days(-7 * 7)) {
        cerr << "days should be " << -7 * 7 << ", but it is " << _days << endl;
        return false;
      }
      cerr << "-7 weeks = " << _days << " days" << endl;
    }

    {
      hours _hours(weeks(1));
      if (_hours != hours(7 * 24)) {
        cerr << "hours should be " << 7 * 24 << ", but it is " << _hours
             << endl;

        return false;
      }
      cerr << "1 week = " << _hours << " hours" << endl;
    }

    {
      hours _hours = weeks(4);
      if (_hours != hours(4 * 7 * 24)) {
        cerr << "hours should be " << 4 * 7 * 24 << ", but it is " << _hours
             << endl;
        return false;
      }
      cerr << "4 weeks  = " << _hours << " hours" << endl;
    }

    {
      hours _hours = weeks(1.5);
      if (_hours != hours(1.5 * static_cast<double>(7) * 24)) {
        cerr << "hours should be " << 1.5 * static_cast<double>(7) * 24
             << ", but it is " << _hours << endl;
        return false;
      }
      cerr << "1.5 weeks  = " << _hours << " hours" << endl;
    }

    {
      hours _hours = weeks(-7);
      if (_hours != hours(static_cast<double>(7) * 24 * -7)) {
        cerr << "hours should be " << -7 * static_cast<double>(7) * 24
             << ", but it is " << _hours << endl;
        return false;
      }
      cerr << "-7 weeks = " << _hours << " hours" << endl;
    }

    {
      minutes _minutes(weeks(1));
      if (_minutes != minutes(7 * 24 * 60)) {
        cerr << "minutes should be " << 7 * 24 * 60 << ", but it is "
             << _minutes << endl;

        return false;
      }
      cerr << "1 week = " << _minutes << " minutes" << endl;
    }

    {
      seconds _seconds(weeks(1));
      if (_seconds != seconds(7 * 24 * 60 * 60)) {
        cerr << "seconds should be " << 7 * 24 * 60 * 60 << ", but it is "
             << _seconds << endl;

        return false;
      }
      cerr << "1 week = " << _seconds << " seconds" << endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'weeks'";
  }
};

struct months_test {

  bool operator()() {

    {
      years _years(months(1));
      if (_years != years(1 / static_cast<double>(12))) {
        cerr << "years should be " << 1 / static_cast<double>(12)
             << ", but it is " << _years << endl;
        return false;
      }
      cerr << "1 month = " << _years << " years" << endl;
    }

    {
      weeks _weeks(months(1));
      if (_weeks != weeks(1 * (365 / static_cast<double>(12)) / 7)) {
        cerr << "weeks should be " << (1 * (365 / static_cast<double>(12)) / 7)
             << ", but it is " << _weeks << endl;
        return false;
      }
      cerr << "1 month = " << _weeks << " weeks" << endl;
    }

    {
      days _days(months(1));
      if (_days != days(1 * (365 / static_cast<double>(12)))) {
        cerr << "days should be " << (1 * (365 / static_cast<double>(12)))
             << ", but it is " << _days << endl;
        return false;
      }
      cerr << "1 month = " << _days << " days" << endl;
    }

    {
      hours _hours(months(1));
      if (_hours != hours(1 * (365 / static_cast<double>(12)) * 24)) {
        cerr << "hours should be " << (1 * (365 / static_cast<double>(12)) * 24)
             << ", but it is " << _hours << endl;
        return false;
      }
      cerr << "1 month = " << _hours << " hours" << endl;
    }

    {
      hours _hours(months(22));
      if (_hours != hours(16060)) {
        cerr << "hours should be " << 16060 << ", but it is " << _hours << endl;
        return false;
      }
      cerr << "1 month = " << _hours << " hours" << endl;
    }

    {
      minutes _minutes(months(1));
      if (_minutes != minutes(60 * 24 * (365 / static_cast<double>(12)))) {
        cerr << "minutes should be "
             << (60 * 24 * (365 / static_cast<double>(12))) << ", but it is "
             << _minutes << endl;
        return false;
      }
      cerr << "1 month = " << _minutes << " minutes" << endl;
    }

    {
      seconds _seconds(months(1));
      if (_seconds != seconds(60 * 60 * 24 * (365 / static_cast<double>(12)))) {
        cerr << "seconds should be "
             << (60 * 60 * 24 * (365 / static_cast<double>(12)))
             << ", but it is " << _seconds << endl;
        return false;
      }
      cerr << "1 month = " << _seconds << " seconds" << endl;
    }

    {
      minutes _minutes(months(2.5));
      if (_minutes !=
          minutes(2.5 * 60 * 24 * (365 / static_cast<double>(12)))) {
        cerr << "minutes should be "
             << (2.5 * 60 * 24 * (365 / static_cast<double>(12)))
             << ", but it is " << _minutes << endl;
        return false;
      }
      cerr << "2.5 months = " << _minutes << " minutes" << endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'months'";
  }
};

struct years_test {

  bool operator()() {

    {
      months _months(years(1));
      if (_months != months(12)) {
        cerr << "months should be 12, but it is " << _months << endl;
        return false;
      }
      cerr << "1 year = " << _months << " months" << endl;
    }

    {
      weeks _weeks(years(1));
      if (_weeks != weeks((365 / static_cast<double>(7)))) {
        cerr << "weeks should be " << (365 / static_cast<double>(7))
             << ", but it is " << _weeks << endl;
        return false;
      }
      cerr << "1 year = " << _weeks << " weeks" << endl;
    }

    {
      days _days(years(1));
      if (_days != days(365)) {
        cerr << "days should be " << 365 << ", but it is " << _days << endl;
        return false;
      }
      cerr << "1 year = " << _days << " days" << endl;
    }

    {
      hours _hours(years(1));
      if (_hours != hours(365 * 24)) {
        cerr << "hours should be " << 365 * 24 << ", but it is " << _hours
             << endl;
        return false;
      }
      cerr << "1 year = " << _hours << " hours" << endl;
    }

    {
      minutes _minutes(years(1));
      if (_minutes != minutes(365 * 24 * 60)) {
        cerr << "minutes should be " << 365 * 24 * 60 << ", but it is "
             << _minutes << endl;
        return false;
      }
      cerr << "1 year = " << _minutes << " minutes" << endl;
    }

    {
      seconds _seconds(years(1));
      if (_seconds != seconds(365 * 24 * 60 * 60)) {
        cerr << "seconds should be " << 365 * 24 * 60 * 60 << ", but it is "
             << _seconds << endl;
        return false;
      }
      cerr << "1 year = " << _seconds << " seconds" << endl;
    }

    return true;
  }

  static constexpr const char *desc() {
    return "Tests conversions from 'years'";
  }
};

int main(int argc, char **argv) {
  cerr.precision(20);
  tenacitas::tester::test _tester(argc, argv);
  run_test(_tester, seconds_test);
  run_test(_tester, minutes_test);
  run_test(_tester, hours_test);
  run_test(_tester, days_test);
  run_test(_tester, weeks_test);
  run_test(_tester, months_test);
  run_test(_tester, years_test);
}
