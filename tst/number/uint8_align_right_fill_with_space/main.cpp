
#include <cstdint>
#include <iostream>
#include <string>

#include <number/format_000.h>
#include <tester/test.h>

using namespace tenacitas;

struct uint8_align_rigth_fill_with_space {
  bool operator()() {
    uint8_t _i(27);

    std::string _s(number::format_000(_i, ' '));

    std::cerr << "s = '" << _s << "'" << std::endl;

    return (_s == " 27");
  }

  static std::string desc() {
    return "Formats a 1 byte number, right aligned and filled with spaces";
  }

  static std::string name() { return "uint8_align_right_fill_with_space"; }
};

int main(int argc, char **argv) {
  tester::test::run<uint8_align_rigth_fill_with_space>(argc, argv);
}
