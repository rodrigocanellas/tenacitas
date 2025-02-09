/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TEST_TUPLE_TRAVERSE_TEST_H
#define TENACITAS_LIB_TUPLE_TEST_TUPLE_TRAVERSE_TEST_H

#include <iostream>
#include <tuple>

#include "tenacitas.lib/program/options.h"
#include "tenacitas.lib/traits/tuple_like.h"
#include "tenacitas.lib/tuple/tuple_traverse.h"

using namespace tenacitas::lib;

namespace tenacitas::lib::tuple::test {

struct tuple_traverse_000 {
  static std::string desc() {
    return "Traverse the values of a 'std::tuple<char>' with 'W'";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple('W')};

    char _char{'-'};

    bool _ret{true};

    auto _visit = [&_ret, &_char]<traits::tuple_like t_tuple, size_t t_idx>(
                      const t_tuple &p_tuple) {
      if constexpr (std::tuple_size_v<t_tuple> == 1) {
        _char = std::get<0>(p_tuple);
        std::cerr << "std::get<0>(p_tuple) = " << _char << std::endl;
        _ret = (_char == 'W');
        if (_ret) {
          return false;
        }
      }
      return true;
    };

    tuple::tuple_mutable_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

    return _ret;
  }
};

struct tuple_traverse_001 {
  static std::string desc() {
    return "Traverse the values of a 'std::tuple<char>' with 'W' and replace "
           "with 'A'";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple('W')};

    char _char{'-'};

    bool _ret{true};

    auto _visit = [&_ret, &_char]<traits::tuple_like t_tuple, size_t t_idx>(
                      t_tuple &p_tuple) {
      if constexpr (std::tuple_size_v<t_tuple> == 1) {
        _char = std::get<0>(p_tuple);
        std::cerr << "std::get<0>(p_tuple) was " << _char;
        _ret = (_char == 'W');
        if (!_ret) {
          return false;
        }
        std::get<0>(p_tuple) = 'A';
        _char = std::get<0>(p_tuple);
        std::cerr << " and now is " << _char << std::endl;
        _ret = (_char == 'A');
      }
      return true;
    };

    tuple::tuple_mutable_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

    return _ret;
  }
};

struct tuple_traverse_002 {
  static std::string desc() {
    return "Checks and prints the types 'typeid' in a tuple";
  }

  bool operator()(const program::options &) {
    using my_tuple = std::tuple<int, char, std::string, float>;

    auto _ret{true};
    auto _visit = [&_ret]<traits::tuple_like t_tuple, std ::size_t t_idx>() {
      using element_type = std::tuple_element_t<t_idx, t_tuple>;
      if constexpr (t_idx == 0) {
        if constexpr (!std::is_same_v<element_type, int>) {
          _ret = false;
          std::cerr << "error checking type in " << t_idx << std::endl;
          return false;
        }
      } else if constexpr (t_idx == 1) {
        if constexpr (!std::is_same_v<element_type, char>) {
          std::cerr << "error checking type in " << t_idx << std::endl;
          _ret = false;
          return false;
        }
      } else if constexpr (t_idx == 2) {
        if constexpr (!std::is_same_v<element_type, std::string>) {
          std::cerr << "error checking type in " << t_idx << std::endl;
          _ret = false;
          return false;
        }
      } else if constexpr (t_idx == 3) {
        if constexpr (!std::is_same_v<element_type, float>) {
          std::cerr << "error checking type in " << t_idx << std::endl;
          _ret = false;
          return false;
        }
      }

      std::cerr << "in idx " << t_idx << " there is a "
                << typeid(element_type).name() << std::endl;

      return true;
    };

    tuple::tuple_type_traverse<my_tuple>(_visit);

    return _ret;
  }
};

} // namespace tenacitas::lib::tuple::test

#endif
