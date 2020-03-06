#include <ctime>

#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <calendar/unix/timestamp.h>
#include <calendar/unix/times.h>
#include <calendar/unix/amounts.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::calendar::unix;

struct test003
{
  bool operator()()
  {
    timestamp _ts1(year(2020), month::mar, day::d01);
    timestamp _ts2 = _ts1 + months(6);

    return (_ts2.get_year() == year(2020) && _ts2.get_month() == month::sep &&
            _ts2.get_day() == day::d01 && _ts2.get_hour() == hour::h00 &&
            _ts2.get_minute() == minute::m00 &&
            _ts2.get_second() == second::s00);
  }
};

int
main(int argc, char** argv)
{
  configure_cerr_log();
  run_test(test003, argc, argv, "01/mar/2020 + 6 months should be 01/sep/2020");
}
