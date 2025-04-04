/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_OSTREAM_TUPLE_H
#define TNCT_OSTREAM_TUPLE_H

#include <iostream>
#include <tuple>

#include "tnct/traits/tuple/like.h"
#include "tnct/tuple/tuple_traverse.h"

template <typename t_char, typename t_traits, typename... t_types>
std::basic_ostream<t_char, t_traits> &
operator<<(std::basic_ostream<t_char, t_traits> &p_out,
           const std::tuple<t_types...> &p_tuple) {

  using tuple = std::tuple<t_types...>;

  p_out << '{';
  auto _visit = [&]<tnct::traits::tuple::like t_tuple_lambda, size_t t_idx>(
                    const t_tuple_lambda &p_t) {
    p_out << std::get<t_idx>(p_t);

    if constexpr (t_idx < (std::tuple_size_v<t_tuple_lambda> - 1)) {
      p_out << ',';
    }
    return true;
  };

  tnct::tuple::tuple_traverse<tuple, decltype(_visit)>(p_tuple, _visit);

  p_out << '}';

  return p_out;
}

template <typename t_char, typename t_traits, tnct::traits::tuple::like t_tuple>
std::basic_ostream<t_char, t_traits> &
operator<<(std::basic_ostream<t_char, t_traits> &p_out,
           const t_tuple &p_tuple) {
  p_out << '{';
  auto _visit = [&]<tnct::traits::tuple::like t_tuple_lambda, size_t t_idx>(
                    const t_tuple_lambda &p_t) {
    p_out << std::get<t_idx>(p_t);

    if constexpr (t_idx < (std::tuple_size_v<t_tuple_lambda> - 1)) {
      p_out << ',';
    }
    return true;
  };

  tnct::tuple::tuple_traverse<t_tuple, decltype(_visit)>(p_tuple, _visit);

  p_out << '}';

  return p_out;
}

#endif
