/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TRATIS_TUPLE_FIND_H
#define TENACITAS_TST_TRATIS_TUPLE_FIND_H

#include <tuple>

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/tuple/traits/find.h"

using namespace tenacitas;

namespace tenacitas::src::traits::tst {

struct tuple_find_000 {
  static std::string desc() {
    return "Tries to find existing simple types in a std::tuple";
  }

  bool operator()(const src::program::options &) {
    using my_tuple = std::tuple<int, char, float>;

    auto _idx{src::tuple::traits::find<my_tuple, int>()};
    if (!_idx) {
      std::cerr << "ERROR! idx should be 0, but the 'int' was not found "
                << std::endl;
      return false;
    }
    auto _index{_idx.value()};
    if (_index != 0) {
      std::cerr << "idx should be 0, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    _idx = src::tuple::traits::find<my_tuple, char>();
    if (!_idx) {
      std::cerr << "ERROR! idx should be 1, but the 'char' was not found "
                << std::endl;
      return false;
    }
    _index = _idx.value();
    if (_index != 1) {
      std::cerr << "idx should be 1, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    _idx = src::tuple::traits::find<my_tuple, float>();
    if (!_idx) {
      std::cerr << "ERROR! idx should be 2, but it 'char' was not found "
                << std::endl;
      return false;
    }
    _index = _idx.value();
    if (_index != 2) {
      std::cerr << "idx should be 2, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    return true;
  }
};

struct tuple_find_001 {
  static std::string desc() {
    return "Tries to find a non existing type in a std::tuple";
  }

  bool operator()(const src::program::options &) {
    using my_tuple = std::tuple<int, char, float>;

    auto _idx{src::tuple::traits::find<my_tuple, std::string>()};
    if (_idx) {
      std::cerr << "'std::string' should not be found, but it is "
                << _idx.value() << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};

struct tuple_find_002 {
  static std::string desc() { return "Tries to find a type in a std::tuple<>"; }

  bool operator()(const src::program::options &) {
    using my_tuple = std::tuple<>;

    auto _idx{src::tuple::traits::find<my_tuple, std::string>()};
    if (_idx) {
      std::cerr << "'std::string' should not be found, but is is "
                << _idx.value() << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};
} // namespace tenacitas::src::traits::tst

#endif
