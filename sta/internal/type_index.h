/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_INTERNAL_TYPE_INDEX_H
#define TENACITAS_LIB_STA_INTERNAL_TYPE_INDEX_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <tuple>
#include <type_traits>

#include <tenacitas.lib/sta/tuple_like.h>

namespace tenacitas::lib::sta::internal {

template <size_t t_idx, typename t_type, tuple_like t_tuple>
constexpr int16_t type_index() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return -1;
  } else if constexpr (std::is_same_v<t_type,
                                      std::tuple_element_t<t_idx, t_tuple>>) {
    return t_idx;
  } else {
    return type_index<t_idx + 1, t_type, t_tuple>();
  }
}

} // namespace tenacitas::lib::sta::internal

#endif
