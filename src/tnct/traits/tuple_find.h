/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_TUPLE_FIND_H
#define TNCT_TUPLE_TUPLE_FIND_H

#include <cstdint>

#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

// template <traits::tuple_like t_tuple, typename t_type, std::size_t t_idx = 0>
// static constexpr std::optional<size_t> tuple_find() {
//   std::optional<size_t> _ret{std::nullopt};
//   if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
//     return _ret;
//   } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
//                                       t_type>) {
//     _ret = {t_idx};
//   } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
//     _ret = {tuple_find<t_tuple, t_type, t_idx + 1>()};
//   }
//   return _ret;
// }

// template <traits::tuple_like t_tuple, typename t_type, std::size_t t_idx = 0>
// static constexpr std::int16_t tuple_find() {
//   if constexpr (std::tuple_size_v<t_tuple> == 0) {
//     return -1;
//   } else if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
//     return static_cast<int16_t>(-1);
//   } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
//                                       t_type>) {
//     return static_cast<int16_t>(t_idx);
//   } else /*if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)*/ {
//     return tuple_find<t_tuple, t_type, t_idx + 1>();
//   }
//   return static_cast<std::int16_t>(-1);
// }

template <typename t_tuple, typename t_type, std::size_t t_idx = 0>
static constexpr std::int16_t tuple_find() {
  if constexpr (std::tuple_size_v<t_tuple> == 0 ||
                t_idx >= std::tuple_size_v<t_tuple>) {
    return static_cast<std::int16_t>(-1);
  } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                      t_type>) {
    return static_cast<std::int16_t>(t_idx);
  } else {
    return tuple_find<t_tuple, t_type, t_idx + 1>();
  }
}

} // namespace tnct::traits

#endif
