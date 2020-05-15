
#include <cstdint>
#include <iostream>
#include <string>

#include <tester/test.h>

using namespace tenacitas;

struct options001 { 
  bool operator()() { 
    // insert your test code here
    return true;
  }

  static std::string desc() { return ""; }

  static std::string name() { return "options001"; }

};

int main(int argc, char **argv) {
  tester::test::run<options001>(argc, argv);  
}

