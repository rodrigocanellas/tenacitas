/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_FIND_H
#define TNCT_TRAITS_TUPLE_FIND_H

#include <optional>

#include "tnct/traits/tuple/like.h"

namespace tnct::traits::tuple {

template <like t_tuple, typename t_type, std::size_t t_idx = 0>
constexpr std::optional<std::size_t> find() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return std::nullopt;
  } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                      t_type>) {
    return t_idx;
  } else {
    return find<t_tuple, t_type, t_idx + 1>(); // Recur to next index
  }
}

// template <like t_tuple, typename t_type, std::size_t t_idx = 0>
// constexpr std::optional<std::size_t> find() {
//   if constexpr (std::tuple_size_v<t_tuple> == 0) {
//     return std::nullopt;
//   } else {
//     std::optional<std::size_t> _maybe_idx;
//     auto _visitor =
//         [&]<tuple::like t_tuple_aux, std::size_t t_idx_aux>() -> bool {
//       using element_type = std::tuple_element_t<t_idx_aux, t_tuple_aux>;
//       if constexpr (std::is_same_v<t_type, element_type>) {
//         _maybe_idx = t_idx_aux;
//         return false;
//       }
//       return true;
//     };

//     tuple::traverse<t_tuple>(_visitor);

//     return _maybe_idx;
//   }
// }

} // namespace tnct::traits::tuple

#endif
