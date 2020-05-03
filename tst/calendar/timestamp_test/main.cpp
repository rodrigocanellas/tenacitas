
#include <cstdint>
#include <iostream>
#include <string>

#include <calendar/gregorian/timestamp.h>
#include <tester/test.h>

using namespace std;

struct timestamp_test {
  bool operator()() {
    using namespace tenacitas::calendar::gregorian;

    {
      timestamp _timestamp;
      cerr << "timestamp = " << _timestamp << endl;
    }

    return true;
  }

  static std::string desc() { return "Basic 'timestamp' tests"; }

  static std::string name() { return "timestamp_test"; }
};

int main(int argc, char **argv) {
  tenacitas::tester::test _test(argc, argv);
  _test.run<timestamp_test>();
}
