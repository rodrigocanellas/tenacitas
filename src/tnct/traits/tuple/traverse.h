/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_TRAVERSE_H
#define TNCT_TRAITS_TUPLE_TRAVERSE_H

#include <cstddef>
// #include <optional>
#include <tuple>

#include "tnct/traits/tuple/like.h"
#include "tnct/traits/tuple/visit.h"

namespace tnct::traits::tuple {

/// \brief Traverses the types of a tuple while the visitor function returns
/// true \note a little help from ChatGPT was used here
template <traits::tuple::like t_tuple, typename t_func, std::size_t t_idx = 0>
requires(traits::tuple::visit_type_in_tuple<t_func, t_tuple, t_idx>)

    constexpr void traverse(t_func p_function) {

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
    return traverse<t_tuple, t_func, t_idx + 1>(p_function);
  }
}

} // namespace tnct::traits::tuple

#endif
