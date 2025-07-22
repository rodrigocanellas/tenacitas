/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TRAITS_STRING_H
#define TENACITAS_TST_TRAITS_STRING_H

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/string/fixed_size_string.h"
#include "tenacitas/src/traits/string/fixed_size_string.h"

using namespace tenacitas;

namespace tenacitas::tst::traits {

struct fixed_size_string_000 {

  static std::string desc() {
    return "Basic traits::src::string::fixed_size_string test";
  }

  bool operator()(const src::program::options &) {
    using fss = src::string::fixed_size_string<5>;

    static_assert(src::traits::string::fixed_size_string<fss>,
                  "src::string::fixed_size_string<5> is conformance to "
                  "traits::src::string::fixed_size_string");

    return true;
  }
};

struct fixed_size_string_001 {

  static std::string desc() {
    return "A non conformance traits::src::string::fixed_size_string test";
  }

  bool operator()(const src::program::options &) {

    using fss = std::string;
    static_assert(!src::traits::string::fixed_size_string<fss>,
                  "src::string::fixed_size_string<5> is conformance to "
                  "traits::src::string::fixed_size_string");

    return true;
  }
};

} // namespace tenacitas::tst::traits
#endif
