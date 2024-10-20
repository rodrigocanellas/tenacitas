/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TUPLE_OUTPUT_H
#define TENACITAS_LIB_TUPLE_TUPLE_OUTPUT_H

#include <iostream>
#include <sstream>
#include <tuple>

#include <tenacitas.lib/tuple/is_tuple.h>
#include <tenacitas.lib/tuple/tuple_like.h>
#include <tenacitas.lib/tuple/tuple_traverse.h>

namespace tenacitas::lib::tuple {

template <typename t_output_stream, tuple::tuple_like t_tuple>
t_output_stream &tuple2out(t_output_stream &p_out, const t_tuple &p_tuple,
                           const char p_left_delimeter = '(',
                           const char p_right_delimeter = ')') {
  p_out << p_left_delimeter;

  auto _visit = [&]<tuple::tuple_like t_tuple_lambda, size_t t_idx>(
                    const t_tuple_lambda &p_t) {
    if constexpr (tuple::is_tuple_v<
                      std::tuple_element_t<t_idx, t_tuple_lambda>>) {
      tuple2out(p_out, std::get<t_idx>(p_t));
    } else {
      p_out << std::get<t_idx>(p_t);
    }
    if constexpr (t_idx < (std::tuple_size_v<t_tuple_lambda> - 1)) {
      p_out << ',';
    }
    return true;
  };

  tuple::tuple_traverse<t_tuple, decltype(_visit)>(p_tuple, _visit);
  p_out << p_right_delimeter;
  return p_out;
}

// template <typename t_tuple>
// requires(tuple::tuple_like<t_tuple>)

//     std::string tuple2str(const t_tuple &p_tuple, const char p_left_delimeter
//     = '(',
//                           const char p_right_delimeter = ')') {
//   std::ostringstream _out;
//   tuple2out(_out, p_tuple, p_left_delimeter, p_right_delimeter);
//   return _out.str();
// }

// template <typename... t_params>
// std::ostream &operator<<(std::ostream &p_out,
//                          const std::tuple<t_params...> &p_tuple) {
//   return tuple2out(p_out, p_tuple);
// }

} // namespace tenacitas::lib::tuple

template <tenacitas::lib::tuple::tuple_like t_tuple>
std::ostream &operator<<(std::ostream &p_out, const t_tuple &p_tuple) {
  return tenacitas::lib::tuple::tuple2out(p_out, p_tuple);
}

template <tenacitas::lib::tuple::tuple_like t_tuple>
std::ostringstream &operator<<(std::ostringstream &p_out,
                               const t_tuple &p_tuple) {
  return tenacitas::lib::tuple::tuple2out(p_out, p_tuple);
}

#endif
