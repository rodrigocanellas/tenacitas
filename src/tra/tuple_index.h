/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRA_TUPLE_INDEX_H
#define TENACITAS_LIB_TRA_TUPLE_INDEX_H

#include <optional>

#include <tenacitas.lib/src/cpt/tuple_like.h>
#include <tenacitas.lib/src/tra/internal/type_index.h>

namespace tenacitas::lib::tra {

template <typename t_type, tlc::tuple_like t_types> struct tuple_index {
  static constexpr std::optional<size_t> value =
      tlt::internal::type_index<0, t_type, t_types>();
};

template <typename t_type, tlc::tuple_like t_types>
inline constexpr std::optional<size_t> tuple_index_v =
    tuple_index<t_type, t_types>::value;

} // namespace tenacitas::lib::tra
#endif
