
#include <tnct/lib/alg/factorial.h>
#include <tnct/lib/alg/log.h>
#include <tnct/lib/alg/program_options.h>
#include <tnct/lib/alg/tester.h>

struct test0000 {
  static std::string desc() { return "factorial of 0"; }
  bool operator()(const tla::program_options &) {
    auto _maybe{tla::factorial<uint16_t>(0)};
    if (!_maybe) {
      TNCT_LOG_ERR("failed to calculate the factorial of 0");
      return false;
    }
    auto _value{_maybe.value()};
    if (_value != 1) {
      TNCT_LOG_ERR("factorial of 0 should be 1, but calculated ", _value);
      return false;
    }
    return true;
  }
};

struct test0001 {
  static std::string desc() { return "factorial of 5"; }
  bool operator()(const tla::program_options &) {
    auto _maybe{tla::factorial<uint16_t>(5)};
    if (!_maybe) {
      TNCT_LOG_ERR("failed to calculate the factorial of 5");
      return false;
    }
    auto _value{_maybe.value()};
    if (_value != 120) {
      TNCT_LOG_ERR("factorial of 5 should be 120, but calculated ", _value);
      return false;
    }
    TNCT_LOG_INF("factorial of 5 is ", _value);
    return true;
  }
};

int main(int argc, char **argv) {

  tla::set_debug_level();
  tla::tester _tester(argc, argv);
  run_test(_tester, test0000);
  run_test(_tester, test0001);
}
