/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TRAVERSE_TUPLE_TRAVERSE_H
#define TENACITAS_LIB_TUPLE_TRAVERSE_TUPLE_TRAVERSE_H

#include <cstddef>
#include <tuple>

#include <tenacitas.lib/tuple/tuple_like.h>

namespace tenacitas::lib::tuple {

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_mutable_tuple = requires(t p_t) {

  requires tuple::tuple_like<t_tuple>;

  {
    p_t.template operator()<t_tuple, t_idx>(std::declval<t_tuple &>())
    } -> std::same_as<bool>;
};

template <tuple::tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(visit_value_in_mutable_tuple<t_func, t_tuple, t_idx>)

    void tuple_traverse(t_tuple &p_tuple, t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple)) {
      return;
    } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
      tuple_traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
    }
  }
}

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_constant_tuple = requires(t p_t) {

  requires tuple::tuple_like<t_tuple>;

  {
    p_t.template operator()<t_tuple, t_idx>(std::declval<const t_tuple &>())
    } -> std::same_as<bool>;
};

template <tuple::tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(visit_value_in_constant_tuple<t_func, t_tuple, t_idx>)

    constexpr void tuple_traverse(const t_tuple &p_tuple, t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple)) {
      return;
    } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
      tuple_traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
    }
  }
}

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_type_in_tuple = requires(t p_t) {

  requires tuple::tuple_like<t_tuple>;

  { p_t.template operator()<t_tuple, t_idx>() } -> std::same_as<bool>;
};

/// \brief Traverses a tuple while the visitor function returns true
template <tuple::tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(visit_type_in_tuple<t_func, t_tuple, t_idx>)

    constexpr void tuple_traverse(t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else if (!p_function.template operator()<t_tuple, t_idx>()) {
    return;
  } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
    return tuple_traverse<t_tuple, t_func, t_idx + 1>(p_function);
  } else {
    return;
  }
}

} // namespace tenacitas::lib::tuple

#endif
