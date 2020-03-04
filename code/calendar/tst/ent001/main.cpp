#include <ctime>

#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <calendar/ent/timestamp.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::calendar::ent;

struct test001
{
  bool operator()()
  {
    return (weekday::wed - weekday::mon).get<uint8_t>() == 2;
  }
};

int
main(int argc, char** argv)
{
  run_test(test001, argc, argv, "'wed' - 'mon' should be 2");
}
