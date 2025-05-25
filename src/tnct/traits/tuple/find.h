/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_FIND_H
#define TNCT_TRAITS_TUPLE_FIND_H

#include <limits>
#include <optional>
#include <type_traits>

#include "tnct/traits/tuple/like.h"
#include "tnct/traits/tuple/visit.h"

namespace tnct::traits::tuple {

template <like t_tuple, typename t_type, std::size_t t_idx = 0>
constexpr std::optional<std::size_t> find() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return std::nullopt;
  } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                      t_type>) {
    return t_idx;
  } else {
    return find<t_tuple, t_type, t_idx + 1>();
  }
}

// template <like t_tuple, typename t_function, std::size_t t_idx = 0>
// requires(visit_type_in_tuple<t_function, t_tuple, t_idx>)

//     constexpr std::size_t find_if(t_function p_function) {
//   if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
//     return std::numeric_limits<std::size_t>::max();
//   } else if constexpr (requires {
//                          p_function.template operator()<t_tuple, t_idx>(
//                              std::declval<const t_tuple &>());
//                        }) {
//     if (!p_function.template operator()<t_tuple, t_idx>()) {
//       return t_idx;
//     }
//   } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
//     return find_if<t_tuple, t_function, t_idx + 1>(p_function);
//   } else {
//     return std::numeric_limits<std::size_t>::max();
//   }
// }

} // namespace tnct::traits::tuple

#endif
