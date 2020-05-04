
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>

#include <calendar/gregorian/timestamp.h>
#include <tester/test.h>

using namespace std;

struct timestamp_now {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    time_t _time = time(nullptr);
    timestamp _timestamp;
    struct tm *_tm = gmtime(&_time);

    if ((_tm->tm_year + 1900) != (_timestamp.get_year().value())) {
      cerr << "year should be " << _tm->tm_year + 1900 << ", but it is "
           << _timestamp.get_year() << endl;
      return false;

      cerr << "timestamp = " << _timestamp << endl;
    }
    cerr << "year = " << _timestamp.get_year() << endl;

    if ((_tm->tm_mon + 1) != (_timestamp.get_month().value())) {
      cerr << "month should be " << (_tm->tm_mon + 1) << ", but it is "
           << _timestamp.get_month().value() << endl;
      return false;
    }
    cerr << "month = " << _timestamp.get_month() << endl;

    if (_tm->tm_mday != _timestamp.get_day().value()) {
      cerr << "day should be " << _tm->tm_mday << ", but it is "
           << _timestamp.get_day().value() << endl;
      return false;
    }
    cerr << "day = " << _timestamp.get_day() << endl;

    if (_tm->tm_hour != _timestamp.get_hour().value()) {
      cerr << "hour should be " << _tm->tm_hour << ", but it is"
           << _timestamp.get_hour().value() << endl;
      return false;
    }
    cerr << "hour = " << _timestamp.get_hour() << endl;

    if (_tm->tm_min != _timestamp.get_minute().value()) {
      cerr << "minute should be " << _tm->tm_min << ", but it is "
           << _timestamp.get_minute().value() << endl;
      return false;
    }
    cerr << "minute = " << _timestamp.get_minute() << endl;

    if (_tm->tm_sec != _timestamp.get_second().value()) {
      cerr << "second should be " << _tm->tm_sec << ", but it is "
           << _timestamp.get_second().value() << endl;
      return false;
    }
    cerr << "second = " << _timestamp.get_second() << endl;

    if ((_tm->tm_wday + 1) != _timestamp.get_weekday().value()) {
      cerr << "weekday should be " << (_tm->tm_wday + 1) << ", but it is "
           << _timestamp.get_weekday().value() << endl;
      return false;
    }

    cerr << "weekday = " << _timestamp.get_weekday() << endl;

    cerr << "timestamp = " << _timestamp << endl;

    return true;
  }

  static std::string desc() { return "Creates a 'timestamp' for now"; }

  static std::string name() { return "timestamp_now"; }
};

struct timestamp_dMyhms_00 {
  bool operator()() {

    using namespace tenacitas::calendar::gregorian;

    timestamp _timestamp(year(2020), month::may, day::_04, hour::_12,
                         minute::_19, second::_23);

    if (_timestamp.get_year() != year(2020)) {
      cerr << "year should be 2020, but it is " << _timestamp.get_year()
           << endl;
      return false;
    }

    cerr << "year = " << _timestamp.get_year() << endl;

    if (month::may != (_timestamp.get_month())) {
      cerr << "month should be " << month::may << ", but it is "
           << _timestamp.get_month() << endl;
      return false;
    }
    cerr << "month = " << _timestamp.get_month() << endl;

    if (day::_04 != _timestamp.get_day()) {
      cerr << "day should be " << day::_04 << ", but it is "
           << _timestamp.get_day() << endl;
      return false;
    }
    cerr << "day = " << _timestamp.get_day() << endl;

    if (hour::_12 != _timestamp.get_hour()) {
      cerr << "hour should be " << hour::_12 << ", but it is"
           << _timestamp.get_hour() << endl;
      return false;
    }
    cerr << "hour = " << _timestamp.get_hour() << endl;

    if (minute::_19 != _timestamp.get_minute()) {
      cerr << "minute should be " << minute::_19 << ", but it is "
           << _timestamp.get_minute() << endl;
      return false;
    }
    cerr << "minute = " << _timestamp.get_minute() << endl;

    if (second::_23 != _timestamp.get_second()) {
      cerr << "second should be " << second::_23 << ", but it is "
           << _timestamp.get_second() << endl;
      return false;
    }
    cerr << "second = " << _timestamp.get_second() << endl;

    if (weekday::mon != _timestamp.get_weekday()) {
      cerr << "weekday should be " << weekday::mon << ", but it is "
           << _timestamp.get_weekday() << endl;
      return false;
    }

    cerr << "weekday " << _timestamp.get_weekday() << endl;

    cerr << "timestamp = " << _timestamp << endl;

    return true;
  }

  static std::string desc() {
    return "Creates a 'timestamp' from 2020-05-04 12:19:23";
  }

  static std::string name() { return "timestamp_dMyhms_00"; }
};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<timestamp_now>();
  _test.run<timestamp_dMyhms_00>();
}
