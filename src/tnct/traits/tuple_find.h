/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_TUPLE_FIND_H
#define TNCT_TUPLE_TUPLE_FIND_H

#include <optional>

#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

template <tuple_like t_tuple, typename t_type, std::size_t t_idx = 0>
static constexpr std::optional<std::size_t> tuple_find() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return std::nullopt;
  } else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                      t_type>) {
    return {t_idx};
  } else {
    return tuple_find<t_tuple, t_type, t_idx + 1>();
  }
}

} // namespace tnct::traits

#endif
