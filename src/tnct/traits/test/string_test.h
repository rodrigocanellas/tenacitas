/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_STRING_TEST_H
#define TNCT_TRAITS_TEST_STRING_TEST_H

#include "tnct/program/options.h"
#include "tnct/string/fixed_size_string.h"
#include "tnct/traits/string/fixed_size_string.h"

using namespace tnct;

namespace tnct::traits::test {

struct fixed_size_string_000 {

  static std::string desc() {
    return "Basic tnct::traits::string::fixed_size_string test";
  }

  bool operator()(const program::options &) {
    using fss = tnct::string::fixed_size_string<5>;

    static_assert(tnct::traits::string::fixed_size_string<fss>,
                  "tnct::string::fixed_size_string<5> is conformance to "
                  "tnct::traits::string::fixed_size_string");

    return true;
  }
};

struct fixed_size_string_001 {

  static std::string desc() {
    return "A non conformance tnct::traits::string::fixed_size_string test";
  }

  bool operator()(const program::options &) {

    using fss = std::string;
    static_assert(!tnct::traits::string::fixed_size_string<fss>,
                  "tnct::string::fixed_size_string<5> is conformance to "
                  "tnct::traits::string::fixed_size_string");

    return true;
  }
};

} // namespace tnct::traits::test
#endif
