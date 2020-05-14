
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
    timestamp _timestamp = timestamp::now();
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

struct timestamp_equal_to {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts1 << endl;

    return (_ts1 == _ts2);

    return true;
  }

  static std::string name() { return "timestamp_equal_to"; }
  static std::string desc() { return "Tests equal-to operator"; }
};

struct timestamp_not_equal_to {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_24);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;

    return (_ts1 != _ts2);

    return true;
  }

  static std::string name() { return "timestamp_not_equal_to"; }
  static std::string desc() { return "Tests not-equal-to operator"; }
};

struct timestamp_epoch_unix {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;
    timestamp _timestamp(year(1970), month::jan, day::_01, hour::_00,
                         minute::_00, second::_00);

    if (_timestamp.get_year() != year(1970)) {
      cerr << "year should be 2020, but it is " << _timestamp.get_year()
           << endl;
      return false;
    }

    cerr << "year = " << _timestamp.get_year() << endl;

    if (month::jan != (_timestamp.get_month())) {
      cerr << "month should be " << month::may << ", but it is "
           << _timestamp.get_month() << endl;
      return false;
    }
    cerr << "month = " << _timestamp.get_month() << endl;

    if (day::_01 != _timestamp.get_day()) {
      cerr << "day should be " << day::_04 << ", but it is "
           << _timestamp.get_day() << endl;
      return false;
    }
    cerr << "day = " << _timestamp.get_day() << endl;

    if (hour::_00 != _timestamp.get_hour()) {
      cerr << "hour should be " << hour::_12 << ", but it is"
           << _timestamp.get_hour() << endl;
      return false;
    }
    cerr << "hour = " << _timestamp.get_hour() << endl;

    if (minute::_00 != _timestamp.get_minute()) {
      cerr << "minute should be " << minute::_19 << ", but it is "
           << _timestamp.get_minute() << endl;
      return false;
    }
    cerr << "minute = " << _timestamp.get_minute() << endl;

    if (second::_00 != _timestamp.get_second()) {
      cerr << "second should be " << second::_23 << ", but it is "
           << _timestamp.get_second() << endl;
      return false;
    }
    cerr << "second = " << _timestamp.get_second() << endl;

    if (weekday::thu != _timestamp.get_weekday()) {
      cerr << "weekday should be " << weekday::mon << ", but it is "
           << _timestamp.get_weekday() << endl;
      return false;
    }

    cerr << "weekday " << _timestamp.get_weekday() << endl;

    cerr << "timestamp = " << _timestamp << endl;
    return true;
  }
  static std::string name() { return "timestamp_epoch_unix"; }
  static std::string desc() {
    return "Calculates timestamp for 1970-01-01 00:00:00";
  }
};

struct timestamp_copy_ctor {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(_ts1);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;

    return (_ts1 == _ts2);
  }

  static std::string name() { return "timestamp_copy_ctor"; }
  static std::string desc() { return "Tests timestamp copy constructor"; }
};

struct timestamp_move_ctor {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(std::move(_ts1));

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;

    return (_ts2 == timestamp(year(2020), month::may, day::_04, hour::_12,
                              minute::_19, second::_23));
  }

  static std::string name() { return "timestamp_move_ctor"; }
  static std::string desc() { return "Tests timestamp move constructor"; }
};

struct timestamp_assign_copy {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2;

    _ts2 = _ts1;

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;

    return (_ts1 == _ts2);
  }
  static std::string name() { return "timestamp_assign_copy "; }
  static std::string desc() { return "Tests assignment copy operator"; }
};

struct timestamp_assign_move {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2;

    _ts2 = std::move(_ts1);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;

    return (_ts2 == timestamp(year(2020), month::may, day::_04, hour::_12,
                              minute::_19, second::_23));
  }

  static std::string name() { return "timestamp_assign_move"; }
  static std::string desc() {
    return "Tests timestamp assignment move operator";
  }
};

struct timestamp_gt {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_24);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;
    return (_ts2 > _ts1);
  }

  static std::string name() { return "timestamp_gt"; }
  static std::string desc() { return "Tests timestamp greater-than operator"; }
};

struct timestamp_lt {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_24);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;
    return (_ts1 < _ts2);
  }

  static std::string name() { return "timestamp_lt"; }
  static std::string desc() { return "Tests timestamp less-than operator"; }
};

struct timestamp_gte {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    {
      timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                     second::_24);

      cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;
      if (!(_ts2 >= _ts1)) {
        return false;
      }
    }

    timestamp _ts3(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 3 = " << _ts3 << endl;
    if (!(_ts3 >= _ts1)) {
      return false;
    }

    return (_ts1 >= _ts1);
  }

  static std::string name() { return "timestamp_gte"; }
  static std::string desc() {
    return "Tests timestamp greater-than-or-equal-to operator";
  }
};

struct timestamp_lte {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    {
      timestamp _ts2(year(2020), month::may, day::_04, hour::_12, minute::_19,
                     second::_24);

      cerr << "timestamp 1 = " << _ts1 << ", timestamp 2 = " << _ts2 << endl;
      if (!(_ts1 <= _ts2)) {
        return false;
      }
    }

    timestamp _ts3(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);

    cerr << "timestamp 1 = " << _ts1 << ", timestamp 3 = " << _ts3 << endl;
    if (!(_ts1 <= _ts3)) {
      return false;
    }

    return (_ts3 <= _ts3);
  }

  static std::string name() { return "timestamp_lte"; }
  static std::string desc() {
    return "Tests timestamp less-than-or-equal-to operator";
  }
};

struct timestamp_add_one_day {
  bool operator()() {

    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);
    timestamp _ts2 = _ts1 + days(1);

    timestamp _target(year(2020), month::may, day::_05, hour::_12, minute::_19,
                      second::_23);

    if (_ts2 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts2 << endl;
      return false;
    }

    cerr << "ts1 =  " << _ts1 << ", ts2 = " << _ts2 << endl;

    return true;
  }
  static std::string name() { return "timestamp_add_one_day"; }
  static std::string desc() { return "Adds one day to 2020-05-04 12:19:23"; }
};

struct timestamp_add_one_week {
  bool operator()() {

    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);
    timestamp _ts2 = _ts1 + weeks(1);

    timestamp _target(year(2020), month::may, day::_11, hour::_12, minute::_19,
                      second::_23);

    if (_ts2 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts2 << endl;
      return false;
    }

    cerr << "ts1 =  " << _ts1 << ", ts2 = " << _ts2 << endl;

    return true;
  }
  static std::string name() { return "timestamp_add_one_week"; }
  static std::string desc() { return "Adds one week to 2020-05-04 12:19:23"; }
};

struct timestamp_add_one_and_a_half_week {
  bool operator()() {

    using namespace tenacitas::calendar::gregorian;

    timestamp _ts1(year(2020), month::may, day::_04, hour::_12, minute::_19,
                   second::_23);
    timestamp _ts2 = _ts1 + weeks(1.5);

    timestamp _target(year(2020), month::may, day::_15, hour::_00, minute::_19,
                      second::_23);

    if (_ts2 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts2 << endl;
      return false;
    }

    cerr << "ts1 =  " << _ts1 << ", ts2 = " << _ts2 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_one_and_a_half_week"; }
  static std::string desc() {
    return "Adds one week and a half week to 2020-05-04 12:19:23";
  }
};

struct timestamp_add_overflow_day_000 {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts0(year(2020), month::may, day::_06, hour::_23, minute::_50,
                   second::_25);

    timestamp _target(year(2020), month::may, day::_08, hour::_22, minute::_38,
                      second::_25);

    timestamp _ts1 = _ts0 + days(1.95);

    if (_ts1 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts1 << endl;
      return false;
    }

    cerr << "ts 0 = " << _ts0 << ", ts 1 = " << _ts1 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_overflow_day_000"; }
  static std::string desc() { return "Adds 1.95 days to 2020-05-06 23:50:25"; }
};

struct timestamp_add_overflow_day_001 {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts0(year(2020), month::may, day::_06, hour::_23, minute::_50,
                   second::_25);

    timestamp _target(year(2020), month::may, day::_07, hour::_00, minute::_03,
                      second::_25);

    timestamp _ts1 = _ts0 + minutes(13);

    if (_ts1 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts1 << endl;
      return false;
    }

    cerr << "ts 0 = " << _ts0 << ", ts 1 = " << _ts1 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_overflow_day_001"; }
  static std::string desc() { return "Adds 13 minutes to 2020-05-06 23:50:25"; }
};

struct timestamp_add_overflow_day_002 {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts0(year(2020), month::may, day::_06, hour::_23, minute::_50,
                   second::_25);

    timestamp _target(year(2020), month::may, day::_07, hour::_00, minute::_06,
                      second::_23);

    timestamp _ts1 = _ts0 + seconds(958);

    if (_ts1 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts1 << endl;
      return false;
    }

    cerr << "ts 0 = " << _ts0 << ", ts 1 = " << _ts1 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_overflow_day_002"; }
  static std::string desc() {
    return "Adds 958 seconds to 2020-05-06 23:50:25";
  }
};

struct timestamp_add_underflow_day_000 {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts0(year(2020), month::may, day::_13, hour::_00, minute::_01,
                   second::_15);

    timestamp _target(year(2020), month::may, day::_12, hour::_23, minute::_56,
                      second::_15);

    timestamp _ts1 = _ts0 - seconds(300);

    // target 1 = days -> 18394 secs -> 86175

    if (_ts1 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts1 << endl;
      return false;
    }

    cerr << "ts 0 = " << _ts0 << ", ts 1 = " << _ts1 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_underflow_day_000"; }
  static std::string desc() {
    return "Substracts 300 seconds from 2020-05-13 00:01:15";
  }
};

struct timestamp_add_underflow_day_001 {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    timestamp _ts0(year(2020), month::may, day::_13, hour::_00, minute::_01,
                   second::_15);

    timestamp _target(year(2020), month::may, day::_12, hour::_14, minute::_25,
                      second::_15);

    timestamp _ts1 = _ts0 - days(0.40);

    if (_ts1 != _target) {
      cerr << "timestamp should be " << _target << ", but is " << _ts1 << endl;
      return false;
    }

    cerr << "ts 0 = " << _ts0 << ", ts 1 = " << _ts1 << endl;

    return true;
  }

  static std::string name() { return "timestamp_add_underflow_day_001"; }
  static std::string desc() {
    return "Substracts 0.4 says from 2020-05-13 00:01:15";
  }
};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<timestamp_now>();
  _test.run<timestamp_dMyhms_00>();
  _test.run<timestamp_equal_to>();
  _test.run<timestamp_not_equal_to>();
  _test.run<timestamp_copy_ctor>();
  _test.run<timestamp_assign_copy>();
  _test.run<timestamp_epoch_unix>();
  _test.run<timestamp_move_ctor>();
  _test.run<timestamp_assign_move>();
  _test.run<timestamp_gt>();
  _test.run<timestamp_lt>();
  _test.run<timestamp_gte>();
  _test.run<timestamp_lte>();
  _test.run<timestamp_add_one_day>();
  _test.run<timestamp_add_one_week>();
  _test.run<timestamp_add_one_and_a_half_week>();
  _test.run<timestamp_add_overflow_day_000>();
  _test.run<timestamp_add_overflow_day_001>();
  _test.run<timestamp_add_overflow_day_002>();
  _test.run<timestamp_add_underflow_day_000>();
  _test.run<timestamp_add_underflow_day_001>();
}
