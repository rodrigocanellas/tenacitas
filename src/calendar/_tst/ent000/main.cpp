#include <ctime>

#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

struct test000 {
  bool operator()() {

    calendar::unix::timestamp _ts;

    time_t _time = time(nullptr);

    struct tm *_tm = localtime(&_time);

    return (
        (_tm->tm_year ==
         calendar::_ent::years(_ts.get_year()).get<decltype(_tm->tm_year)>() -
             1900) &&
        (_tm->tm_mon == calendar::_ent::months(_ts.get_month())
                            .get<decltype(_tm->tm_mon)>()) &&
        (_tm->tm_mday ==
         calendar::_ent::days(_ts.get_day()).get<decltype(_tm->tm_mday)>()) &&
        (_tm->tm_hour ==
         calendar::_ent::hours(_ts.get_hour()).get<decltype(_tm->tm_hour)>()) &&
        (_tm->tm_min == calendar::_ent::minutes(_ts.get_minute())
                            .get<decltype(_tm->tm_min)>()) &&
        (_tm->tm_sec == calendar::_ent::seconds(_ts.get_second())
                            .get<decltype(_tm->tm_sec)>()));
  }

  static std::string desc() {
    return "Compares \"now\" timestamp create with 'timestamp' and with "
           "'time'.";
  }
  static std::string name() { return "test000"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<test000>(argc, argv);
}
