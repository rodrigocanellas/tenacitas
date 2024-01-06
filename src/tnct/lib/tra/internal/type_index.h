/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_TRA_INTERNAL_TYPE_INDEX_H
#define TNCT_LIB_TRA_INTERNAL_TYPE_INDEX_H

#include <cstddef>
#include <cstring>
#include <ctime>
#include <optional>

#include <tuple>
#include <type_traits>

#include <tnct/lib/cpt/tuple_like.h>

namespace tnct::lib::tra::internal {

template <size_t t_idx, typename t_type, tlc::tuple_like t_tuple>
constexpr std::optional<size_t> type_index() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return {};
  } else if constexpr (std::is_same_v<t_type,
                                      std::tuple_element_t<t_idx, t_tuple>>) {
    return {t_idx};
  } else {
    return type_index<t_idx + 1, t_type, t_tuple>();
  }
}

} // namespace tnct::lib::tra::internal

namespace tlt = tnct::lib::tra;

#endif
