#include <calendar/_bus/add.h>
#include <calendar/_bus/daily_repetition.h>
#include <calendar/_ent/day.h>
#include <calendar/_ent/days.h>
#include <calendar/_ent/hour.h>
#include <calendar/_ent/minute.h>
#include <calendar/_ent/month.h>
#include <calendar/_ent/second.h>
#include <calendar/_ent/year.h>
#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct day_sequence_000 {
  bool operator()() {

    typedef calendar::unix::timestamp timestamp;
    //    typedef calendar::_bus::daily_repetition_t<timestamp>
    //    daily_repetition;
    typedef calendar::_ent::day day;
    typedef calendar::_ent::month month;
    typedef calendar::_ent::year year;
    typedef calendar::_ent::hour hour;
    typedef calendar::_ent::minute minute;
    typedef calendar::_ent::second second;

    typedef logger::cerr::log log;
    typedef calendar::_ent::days days;

    //    daily_repetition _daily_repetition(4);

    timestamp _first(year(2020), month::mar, day::d10, hour::h00, minute::m00,
                     second::s00);
    timestamp _second = calendar::_bus::add_days(_first, days(4));

    //    timestamp _second = _daily_repetition.next(_first);

    log::debug("", __LINE__, _first, _second);

    return _second == timestamp(year(2020), month::mar, day::d14);
  }

  static std::string desc() { return "Calculate 4 days after 2020/mar/10"; }

  static std::string name() { return "day_sequence_000"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<day_sequence_000>(argc, argv);
}
