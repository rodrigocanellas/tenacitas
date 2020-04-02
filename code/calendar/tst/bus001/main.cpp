#include <ctime>

#include <calendar/ent/day.h>
#include <calendar/ent/month.h>
#include <calendar/ent/months.h>
#include <calendar/ent/year.h>
#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

struct test003 {
  bool operator()() {

    calendar::unix::timestamp _ts1(calendar::ent::year(2020),
                                   calendar::ent::month::mar,
                                   calendar::ent::day::d01);

    calendar::unix::timestamp _ts2 = _ts1 + calendar::ent::months(6);

    return (_ts2.get_year() == calendar::ent::year(2020) &&
            _ts2.get_month() == calendar::ent::month::sep &&
            _ts2.get_day() == calendar::ent::day::d01 &&
            _ts2.get_hour() == calendar::ent::hour::h00 &&
            _ts2.get_minute() == calendar::ent::minute::m00 &&
            _ts2.get_second() == calendar::ent::second::s00);
  }

  static std::string desc() {
    return "01/mar/2020 + 6 months should be 01/sep/2020";
  }
  static std::string name() { return "test003"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<test003>(argc, argv);
}
