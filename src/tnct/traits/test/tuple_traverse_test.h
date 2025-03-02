/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_TUPLE_TRAVERSE_TEST_H
#define TNCT_TRAITS_TEST_TUPLE_TRAVERSE_TEST_H

#include <iostream>
#include <tuple>

#include "tnct/program/options.h"
#include "tnct/traits/tuple/like.h"
#include "tnct/traits/tuple/traverse.h"

using namespace tnct;

namespace tnct::traits::test {

struct tuple_traverse_000 {
  static std::string desc() {
    return "Checks and prints the types 'typeid' in a tuple";
  }

  bool operator()(const program::options &) {
    using my_tuple = std::tuple<int, char, std::string, float>;

    auto _ret{true};
    auto _visit = [&_ret]<traits::tuple::like t_tuple, std ::size_t t_idx>() {
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

    traits::tuple::traverse<my_tuple>(_visit); // traverse<my_tuple>(_visit);

    return _ret;
  }
};

} // namespace tnct::traits::test

#endif
