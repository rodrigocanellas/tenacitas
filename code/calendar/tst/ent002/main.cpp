#include <ctime>

#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <calendar/unix/timestamp.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::calendar::ent;

struct test002
{
  bool operator()()
  {
    return (weekday::mon - weekday::wed).get<uint8_t>() == 5;
  }
};

int
main(int argc, char** argv)
{
  configure_cerr_log();
  run_test(test002, argc, argv, "'mon' - 'wed' should be 5");
}
