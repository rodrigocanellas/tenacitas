/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TEST_TUPLE_TRAVERSE_TEST_H
#define TNCT_TUPLE_TEST_TUPLE_TRAVERSE_TEST_H

#include <iostream>
#include <tuple>

#include "tnct/program/options.h"
#include "tnct/traits/tuple/like.h"
#include "tnct/tuple/tuple_traverse.h"

using namespace tnct;

namespace tnct::tuple::test {

struct tuple_traverse_000 {
  static std::string desc() {
    return "Traverse the values of a 'std::tuple<char>' with 'W'";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple('W')};

    char _char{'-'};

    bool _ret{true};

    auto _visit = [&_ret, &_char]<traits::tuple::like t_tuple, size_t t_idx>(
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

    tuple::tuple_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

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

    auto _visit = [&_ret, &_char]<traits::tuple::like t_tuple, size_t t_idx>(
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

    tuple::tuple_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

    return _ret;
  }
};

struct tuple_traverse_003 {
  static std::string desc() {
    return "Traverse 'std::tuple<char, int, float>' with 'W', -9 and 1.14 and "
           "prints the values";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple('W', -9)};

    auto _visit =
        []<traits::tuple::like t_tuple, size_t t_idx>(const t_tuple &p_tuple) {
          std::cerr << "in " << t_idx << " there is "
                    << std::get<t_idx>(p_tuple) << '\n';
          return true;
        };

    tuple::tuple_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

    return true;
  }
};

struct tuple_traverse_004 {
  static std::string desc() {
    return "Traverse 'std::tuple<uint16_t, float, int32_t>' with '14', 3.14 "
           "and -983271, adds 1 to each of them and prints the values";
  }

  bool operator()(const program::options &) {
    auto _tuple{std::make_tuple(14U, 3.14, -983271)};

    auto _visit =
        []<traits::tuple::like t_tuple, size_t t_idx>(t_tuple &p_tuple) {
          std::cerr << "in " << t_idx << " there was "
                    << std::get<t_idx>(p_tuple) << ", and now there is "
                    << (std::get<t_idx>(p_tuple) = std::get<t_idx>(p_tuple) + 1)
                    << '\n';
          return true;
        };

    tuple::tuple_traverse<decltype(_tuple), decltype(_visit)>(_tuple, _visit);

    return true;
  }
};

} // namespace tnct::tuple::test

#endif
