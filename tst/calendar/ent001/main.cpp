#include <ctime>

#include <calendar/_ent/weekday.h>
#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct test001 {
  bool operator()() {
    return (calendar::_ent::weekday::wed - calendar::_ent::weekday::mon)
               .get<uint8_t>() == 2;
  }

  static std::string desc() { return "'wed' - 'mon' should be 2"; }
  static std::string name() { return "test001"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<test001>(argc, argv);
}