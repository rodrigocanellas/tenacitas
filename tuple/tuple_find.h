/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TUPLE_FIND_H
#define TENACITAS_LIB_TUPLE_TUPLE_FIND_H

#include <optional>

#include <tenacitas.lib/tuple/tuple_like.h>

namespace tenacitas::lib::tuple {

template <tuple::tuple_like t_tuple, typename t_type, std::size_t t_idx = 0>
static constexpr std::optional<size_t> tuple_find() {
  std::optional<size_t> _ret{std::nullopt};
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return _ret;
  } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                      t_type>) {
    _ret = {t_idx};
  } else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>) {
    _ret = {tuple_find<t_tuple, t_type, t_idx + 1>()};
  }
  return _ret;
}

} // namespace tenacitas::lib::tuple

#endif
