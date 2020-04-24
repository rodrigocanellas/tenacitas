
#include <cstdint>
#include <iostream>
#include <string>

#include <tester/test.h>

using namespace tenacitas;

struct day_test { 
  bool operator()() { 
    // insert your test code here
    return true;
  }

  static std::string desc() { return ""; }

  static std::string name() { return "day_test"; }

};

int main(int argc, char **argv) {
  tester::test::run<day_test>(argc, argv);  
}

