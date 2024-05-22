/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TRAVERSE_TUPLE_TRAVERSE_H
#define TENACITAS_LIB_TRAITS_TRAVERSE_TUPLE_TRAVERSE_H

#include <cstddef>
#include <cstdint>
#include <tuple>

#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits {

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_tuple = requires(t p_t) {

  requires tuple_like<t_tuple>;

  {
    p_t.template operator()<t_idx>(std::declval<t_tuple &>())
    } -> std::same_as<bool>;
};

template <typename t_func, tuple_like t_tuple, size_t t_idx = 0>
requires(visit_value_in_tuple<t_func, t_tuple, t_idx>)

    void tuple_traverse(t_func p_function, t_tuple &p_tuple) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else {
    if (!p_function.template operator()<t_idx>(p_tuple)) {
      return;
    } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
      tuple_traverse<t_func, t_tuple, t_idx + 1>(p_function, p_tuple);
    }
  }
}

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_type_in_tuple = requires(t p_t) {

  requires tuple_like<t_tuple>;

  { p_t.template operator()<t_tuple, t_idx>() } -> std::same_as<bool>;
};

/// \brief Traverses a tuple while the visitor function returns true
template <tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(visit_type_in_tuple<t_func, t_tuple, t_idx>)

    constexpr int16_t tuple_traverse(t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return -1;
  } else if (!p_function.template operator()<t_tuple, t_idx>()) {
    return t_idx;
  } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
    return tuple_traverse<t_tuple, t_func, t_idx + 1>(p_function);
  } else {
    return -1;
  }
}

} // namespace tenacitas::lib::traits

#endif
