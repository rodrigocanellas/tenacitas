#include <cstdint>
#include <string>

#include <number/format_000.h>
#include <tester/test.h>

using namespace tenacitas;

struct int16_align_left_fill_with_zero {
  bool operator()() {

    int16_t _i(-2);

    std::string _s(number::format_000(_i));

    std::cerr << "s = '" << _s << "'" << std::endl;

    return (_s == "000-2");
  }

  static std::string desc() {
    return "Formats a 2 byte number, right aligned and filled with zeros";
  }

  static std::string name() { return "int16_align_left_fill_with_zero"; }
};

int main(int argc, char **argv) {
  tester::test::run<int16_align_left_fill_with_zero>(argc, argv);
}
