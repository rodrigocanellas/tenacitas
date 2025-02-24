/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_TRAVERSE_TUPLE_TRAVERSE_H
#define TNCT_TUPLE_TRAVERSE_TUPLE_TRAVERSE_H

#include <cstddef>
#include <tuple>

#include "tnct/traits/tuple_like.h"
#include "tnct/traits/visit_tuple.h"

namespace tnct::tuple {

template <traits::tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(traits::visit_value_in_constant_tuple<t_func, t_tuple, t_idx>)

    constexpr void tuple_constant_traverse(const t_tuple &p_tuple,
                                           t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple)) {
      return;
    } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
      tuple_constant_traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
    }
  }
}

template <traits::tuple_like t_tuple, typename t_func, size_t t_idx = 0>
requires(traits::visit_value_in_mutable_tuple<t_func, t_tuple, t_idx>)

    constexpr void tuple_mutable_traverse(t_tuple &p_tuple, t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  } else {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple)) {
      return;
    } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
      tuple_constant_traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
    }
  }
}

/// \brief Traverses a tuple while the visitor function returns true
/// \note a little help from ChatGPT was used here
template <traits::tuple_like t_tuple, typename t_func, std::size_t t_idx = 0>
requires(traits::visit_type_in_tuple<t_func, t_tuple, t_idx>)

    constexpr void tuple_traverse(t_func p_function) {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return;
  }

  if constexpr (requires {
                  p_function.template operator()<t_tuple, t_idx>();
                }) {
    if (!p_function.template operator()<t_tuple, t_idx>())
      return;
  }

  if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
    tuple_traverse<t_tuple, t_func, t_idx + 1>(p_function);
  }
}

} // namespace tnct::tuple

#endif
