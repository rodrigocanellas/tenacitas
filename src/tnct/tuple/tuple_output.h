/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_TUPLE_OUTPUT_H
#define TNCT_TUPLE_TUPLE_OUTPUT_H

#include <iostream>
#include <tuple>


// #include "tenacitas/src/traits/is_tuple.h"
// #include "tenacitas/src/traits/tuple_like.h"
// #include "tenacitas/src/tuple/tuple_traverse.h"

// namespace tnct::tuple {

// template <typename t_output_stream, traits::tuple_like t_tuple>
// t_output_stream &tuple2out(t_output_stream &p_out, const t_tuple &p_tuple,
//                            const char p_left_delimeter = '(',
//                            const char p_right_delimeter = ')') {
//   p_out << p_left_delimeter;

//   auto _visit = [&]<traits::tuple_like t_tuple_lambda, size_t t_idx>(
//                     const t_tuple_lambda &p_t) {
//     if constexpr (traits::is_tuple_v<
//                       std::tuple_element_t<t_idx, t_tuple_lambda>>) {
//       tuple2out(p_out, std::get<t_idx>(p_t));
//     } else {
//       p_out << std::get<t_idx>(p_t);
//     }
//     if constexpr (t_idx < (std::tuple_size_v<t_tuple_lambda> - 1)) {
//       p_out << ',';
//     }
//     return true;
//   };

//   tuple::tuple_constant_traverse<t_tuple, decltype(_visit)>(p_tuple, _visit);
//   p_out << p_right_delimeter;
//   return p_out;
// }

// } // namespace tnct::tuple

template <typename t_char, typename t_traits, typename... t_types>
std::basic_ostream<t_char, t_traits> &
operator<<(std::basic_ostream<t_char, t_traits> &p_out,
           const std::tuple<t_types...> &p_tuple) {
  std::apply(
      [&p_out](const t_types &...p_types) {
        p_out << '[';
        std::size_t n{0};
        ((p_out << p_types << (++n != sizeof...(t_types) ? ", " : "")), ...);
        p_out << ']';
      },
      p_tuple);
  return p_out;
}

#endif
