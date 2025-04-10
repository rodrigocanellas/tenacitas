/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_TUPLE_IS_TYPE_IN_TUPLE_TEST_H
#define TNCT_TRAITS_TEST_TUPLE_IS_TYPE_IN_TUPLE_TEST_H

#include <tuple>

#include "tnct/program/options.h"
#include "tnct/traits/tuple/contains_type.h"

using namespace tnct;

namespace tnct::traits::test {

struct is_type_in_tuple_000 {
  static std::string desc() {
    return "Tries to find existing simple types in a std::tuple";
  }

  bool operator()(const program::options &) {
    using my_tuple = std::tuple<int, char, float>;

    auto _found{traits::tuple::contains_type<my_tuple, int>()};
    if (!_found) {
      std::cerr << "ERROR! 'int' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'int' found, as expected" << std::endl;

    _found = traits::tuple::contains_type<my_tuple, char>();
    if (!_found) {
      std::cerr << "ERROR! 'char' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'char' found, as expected" << std::endl;

    _found = traits::tuple::contains_type<my_tuple, float>();
    if (!_found) {
      std::cerr << "ERROR! 'flaot' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'float' found, as expected" << std::endl;
    return true;
  }
};

struct is_type_in_tuple_001 {
  static std::string desc() {
    return "Tries to find a non existing type in a std::tuple";
  }

  bool operator()(const program::options &) {
    using my_tuple = std::tuple<int, char, float>;

    auto _found{traits::tuple::contains_type<my_tuple, std::string>()};
    if (_found) {
      std::cerr << "'std::string' should not be found, but is was" << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};

struct is_type_in_tuple_002 {
  static std::string desc() { return "Tries to find a type in a std::tuple<>"; }

  bool operator()(const program::options &) {
    using my_tuple = std::tuple<>;

    auto _found{traits::tuple::contains_type<my_tuple, std::string>()};
    if (_found) {
      std::cerr << "'std::string' should not be found, but is was" << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};
} // namespace tnct::traits::test

#endif
