#include <ctime>

#include <calendar/_ent/day.h>
#include <calendar/_ent/month.h>
#include <calendar/_ent/months.h>
#include <calendar/_ent/year.h>
#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct test003 {
  bool operator()() {

    calendar::unix::timestamp _ts1(calendar::_ent::year(2020),
                                   calendar::_ent::month::mar,
                                   calendar::_ent::day::d01);

    calendar::unix::timestamp _ts2 = _ts1 + calendar::_ent::months(6);

    return (_ts2.get_year() == calendar::_ent::year(2020) &&
            _ts2.get_month() == calendar::_ent::month::sep &&
            _ts2.get_day() == calendar::_ent::day::d01 &&
            _ts2.get_hour() == calendar::_ent::hour::h00 &&
            _ts2.get_minute() == calendar::_ent::minute::m00 &&
            _ts2.get_second() == calendar::_ent::second::s00);
  }

  static std::string desc() {
    return "01/mar/2020 + 6 months should be 01/sep/2020";
  }
  static std::string name() { return "test003"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<test003>(argc, argv);
}
