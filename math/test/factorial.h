/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/generic/program_options.h>
#include <tenacitas.lib/log/cerr.h>
#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/math/factorial.h>

using namespace tenacitas::lib;

struct test0000 {
  static std::string desc() { return "factorial of 0"; }
  bool operator()(const generic::program_options &) {
    log::cerr _cerr;
    auto _maybe{mat::factorial<uint16_t>(0)};
    if (!_maybe) {
      _cerr.err("failed to calculate the factorial of 0");
      return false;
    }
    auto _value{_maybe.value()};
    if (_value != 1) {
      _cerr.err(
          generic::fmt("factorial of 0 should be 1, but calculated ", _value));
      return false;
    }
    return true;
  }
};

struct test0001 {
  static std::string desc() { return "factorial of 5"; }
  bool operator()(const generic::program_options &) {
    log::cerr _cerr;
    auto _maybe{mat::factorial<uint16_t>(5)};
    if (!_maybe) {
      _cerr.err("failed to calculate the factorial of 5");
      return false;
    }
    auto _value{_maybe.value()};
    if (_value != 120) {
      _cerr.err(
          generic::fmt("factorial of 5 should be 120, but calculated ", _value));
      return false;
    }
    _cerr.tst(generic::fmt("factorial of 5 is ", _value));
    return true;
  }
};
