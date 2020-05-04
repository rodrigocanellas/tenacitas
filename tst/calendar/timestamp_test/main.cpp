
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>

#include <calendar/gregorian/timestamp.h>
#include <tester/test.h>

using namespace std;

struct timestamp_now {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      time_t _time = time(nullptr);
      timestamp _timestamp;
      struct tm *_tm = gmtime(&_time);

      if ((_tm->tm_year + 1900) != (_timestamp.get_year().value())) {
        cerr << "year should be " << _tm->tm_year + 1900 << ", but it is "
             << _timestamp.get_year() << endl;
        return false;
      }

      cerr << "timestamp = " << _timestamp << endl;
    }

    return true;
  }

  static std::string desc() { return "Create a 'timestamp' for now"; }

  static std::string name() { return "timestamp_now"; }
};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<timestamp_now>();
}
