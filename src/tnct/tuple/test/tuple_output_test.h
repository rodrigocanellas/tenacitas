/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TEST_TUPLE_OUTPUT_TEST_H
#define TNCT_TUPLE_TEST_TUPLE_OUTPUT_TEST_H

#include <iostream>
#include <limits>
#include <string>
#include <tuple>

#include "tnct/program/options.h"
#include "tnct/tuple/tuple_output.h"

using namespace tnct;

namespace tnct::tuple::test {

struct tuple_output_000 {
  static std::string desc() { return "Simple tuple output test"; }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple('c', 3.14, -8)};

    std::cerr << "tuple = " << _tuple << std::endl;
    return true;
  }
};

struct tuple_output_001 {
  static std::string desc() { return "Printing a tuple with a tuple inside"; }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple(
        'c', 3.14,
        std::make_tuple("hello", std::numeric_limits<uint32_t>::max(), 0.0029),
        -8)};

    std::cerr << "tuple = " << _tuple << std::endl;
    return true;
  }
};

struct tuple_output_002 {
  static std::string desc() {
    return "Printing a tuple with a tuple and a pair  inside";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple(
        'c', 3.14,
        std::make_tuple("hello", std::numeric_limits<uint32_t>::max(), 0.0029),
        -8, std::make_pair('K', 33))};

    std::cerr << "tuple = " << _tuple << std::endl;
    return true;
  }
};

} // namespace tnct::tuple::test

#endif
