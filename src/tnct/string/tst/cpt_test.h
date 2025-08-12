/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_STRING_TST_CPT_TEST_H
#define TNCT_STRING_TST_CPT_TEST_H

#include "tnct/program/options.h"
#include "tnct/string/cpt/fixed_size_string.h"
#include "tnct/string/fixed_size_string.h"

using namespace tnct;

namespace tnct::string::tst
{

struct cpt_fixed_size_string_000
{

  static std::string desc()
  {
    return "Basic concepts::string::fixed_size_string test";
  }

  bool operator()(const program::options &)
  {
    using fss = string::fixed_size_string<5>;

    static_assert(string::cpt::fixed_size_string<fss>,
                  "string::fixed_size_string<5> is conformance to "
                  "concepts::string::fixed_size_string");

    return true;
  }
};

struct cpt_fixed_size_string_001
{

  static std::string desc()
  {
    return "A non conformance concepts::string::fixed_size_string test";
  }

  bool operator()(const program::options &)
  {

    using fss = std::string;
    static_assert(!string::cpt::fixed_size_string<fss>,
                  "string::fixed_size_string<5> is conformance to "
                  "concepts::string::fixed_size_string");

    return true;
  }
};

} // namespace tnct::string::tst
#endif
