#include <ctime>

#include <calendar/unix/timestamp.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

using namespace tenacitas;

struct test002 {
  bool operator()() {
    return (calendar::ent::weekday::mon - calendar::ent::weekday::wed)
               .get<uint8_t>() == 5;
  }
  static std::string desc() { return "'mon' - 'wed' should be 5"; }
  static std::string name() { return "test002"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<test002>(argc, argv);
}
