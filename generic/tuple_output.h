/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TUPLE_OUTPUT_H
#define TENACITAS_LIB_TRAITS_TUPLE_OUTPUT_H

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include <tenacitas.lib/traits/is_pair.h>
#include <tenacitas.lib/traits/is_tuple.h>
#include <tenacitas.lib/traits/pair_like.h>
#include <tenacitas.lib/traits/tuple_like.h>
#include <tenacitas.lib/traits/tuple_traverse.h>

namespace tenacitas::lib::generic {

template <typename t_first, typename t_second>
std::ostream &
pair2out(std::ostream &p_out, const std::pair<t_first, t_second> &p_pair,
         const char p_left_delimeter = '(', const char p_right_delimeter = ')');

template <typename t_tuple>
requires(traits::tuple_like<t_tuple>) std::ostream &tuple2out(
    std::ostream &p_out, const t_tuple &p_tuple,
    const char p_left_delimeter = '(', const char p_right_delimeter = ')') {
  p_out << p_left_delimeter;
  traits::tuple_type_traverse(
      [&]<size_t t_idx>(const t_tuple &p_t) {
        if constexpr (traits::is_tuple_v<
                          std::tuple_element_t<t_idx, t_tuple>>) {
          tuple2out(p_out, std::get<t_idx>(p_t));
        } else if constexpr (traits::is_pair_v<
                                 std::tuple_element_t<t_idx, t_tuple>>) {
          pair2out(p_out, std::get<t_idx>(p_t));
        } else {
          p_out << std::get<t_idx>(p_t);
        }
        if constexpr (t_idx < (std::tuple_size_v<t_tuple> - 1)) {
          p_out << ',';
        }
        return true;
      },
      p_tuple);
  p_out << p_right_delimeter;
  return p_out;
}

template <typename t_tuple>
requires(traits::tuple_like<t_tuple>) std::string
    tuple2str(const t_tuple &p_tuple, const char p_left_delimeter = '(',
              const char p_right_delimeter = ')') {
  std::ostringstream _out;
  tuple2out(_out, p_tuple, p_left_delimeter, p_right_delimeter);
  return _out.str();
}

template <typename t_first, typename t_second>
std::ostream &
pair2out(std::ostream &p_out, const std::pair<t_first, t_second> &p_pair,
         const char p_left_delimeter, const char p_right_delimeter) {
  using pair = const std::pair<t_first, t_second>;
  p_out << p_left_delimeter;
  if constexpr (traits::is_tuple_v<typename pair::first_type>) {
    tuple2out(p_out, p_pair.first);
  } else {
    p_out << p_pair.first;
  }
  p_out << ',';
  if constexpr (traits::is_tuple_v<typename pair::second_type>) {
    tuple2out(p_out, p_pair.second);
  } else {
    p_out << p_pair.second;
  }
  p_out << p_right_delimeter;
  return p_out;
}

template <typename t_first, typename t_second>
std::string pair2str(const std::pair<t_first, t_second> &p_pair,
                     const char p_left_delimeter = '(',
                     const char p_right_delimeter = ')') {
  std::ostringstream _out;
  pair2out(_out, p_pair, p_left_delimeter, p_right_delimeter);
  return _out.str();
}

} // namespace tenacitas::lib::generic

template <typename... t_params>
std::ostream &operator<<(std::ostream &p_out,
                         const std::tuple<t_params...> &p_tuple) {
  return tenacitas::lib::generic::tuple2out(p_out, p_tuple);
}

template <typename t_first, typename t_second>
std::ostream &operator<<(std::ostream &p_out,
                         const std::pair<t_first, t_second> &p_pair) {
  return tenacitas::lib::generic::pair2out(p_out, p_pair);
}

#endif
