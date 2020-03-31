#include <ctime>

#include <logger/cerr/log.h>

#include <tester/bus/test.h>

#include <calendar/unix/amounts.h>
#include <calendar/unix/times.h>
#include <calendar/unix/timestamp.h>

using namespace tenacitas;

struct test003 {
  bool operator()() {
    calendar::unix::timestamp _ts1(calendar::unix::year(2020),
                                   calendar::unix::month::mar,
                                   calendar::unix::day::d01);
    calendar::unix::timestamp _ts2 = _ts1 + calendar::ent::months(6);

    return (_ts2.get_year() == calendar::unix::year(2020) &&
            _ts2.get_month() == calendar::unix::month::sep &&
            _ts2.get_day() == calendar::unix::day::d01 &&
            _ts2.get_hour() == calendar::unix::hour::h00 &&
            _ts2.get_minute() == calendar::unix::minute::m00 &&
            _ts2.get_second() == calendar::unix::second::s00);
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
