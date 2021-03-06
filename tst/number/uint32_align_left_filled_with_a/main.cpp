/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstdint>
#include <iostream>
#include <string>

#include <number/format_000.h>
#include <tester/test.h>

using namespace tenacitas;

struct uint32_align_left_filled_with_a {
  bool operator()() {
    uint32_t _i(1024);

    std::string _s(number::format_000(_i, 'a', number::align::left));

    std::cerr << "s = '" << _s << "'" << std::endl;

    return (_s == "1024aaaaaa");
  }

  static std::string desc() {
    return "Formats a 4 byte number, left aligned and filled with 'a'";
  }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);

  run_test(_tester, uint32_align_left_filled_with_a);
}
