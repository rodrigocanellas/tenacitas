#include <ctime>

#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <calendar/entities/timestamp_second.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::calendar::entities;

struct test000
{
  bool operator()()
  {

    timestamp<second> _ts;

    time_t _time = time(nullptr);

    struct tm* _tm = localtime(&_time);

    return (
      (_tm->tm_year ==
       amount<year>(_ts.get_year()).get<decltype(_tm->tm_year)>() - 1900) &&
      (_tm->tm_mon ==
       amount<month>(_ts.get_month()).get<decltype(_tm->tm_mon)>()) &&
      (_tm->tm_mday ==
       amount<day>(_ts.get_day()).get<decltype(_tm->tm_mday)>()) &&
      (_tm->tm_hour ==
       amount<hour>(_ts.get_hour()).get<decltype(_tm->tm_hour)>()) &&
      (_tm->tm_min ==
       amount<minute>(_ts.get_minute()).get<decltype(_tm->tm_min)>()) &&
      (_tm->tm_sec ==
       amount<second>(_ts.get_second()).get<decltype(_tm->tm_sec)>()));
  };
};

struct test001
{
  bool operator()()
  {
    return (weekday::wed - weekday::mon).get<uint8_t>() == 2;
  };
};

struct test002
{
  bool operator()()
  {
    return (weekday::mon - weekday::wed).get<uint8_t>() == 5;
  };
};

struct test003
{
  bool operator()()
  {
    timestamp<second> _ts1(year(2020), month::mar, day::d01);
    timestamp<second> _ts2 = _ts1 + amount<month>(6);

    return (_ts2.get_year() == year(2020) && _ts2.get_month() == month::sep &&
            _ts2.get_day() == day::d01 && _ts2.get_hour() == hour::h00 &&
            _ts2.get_minute() == minute::m00 &&
            _ts2.get_second() == second::s00);
  };
};

int
main(int argc, char** argv)
{
  configure_cerr_log();
  run_test(
    test000,
    argc,
    argv,
    "Compares \"now\" timestamp create with 'timestamp' and with 'time'.");
  run_test(test001, argc, argv, "'wed' - 'mon' should be 2");
  run_test(test002, argc, argv, "'mon' - 'wed' should be 5");
  run_test(test003, argc, argv, "01/mar/2020 + 6 months should be 01/sep/2020");
}
