#include <ctime>

#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <calendar/ent/timestamp.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::calendar::ent;

struct test000
{
  bool operator()()
  {

    timestamp _ts;

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
  }
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
}
