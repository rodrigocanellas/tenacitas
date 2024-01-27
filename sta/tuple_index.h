/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_TUPLE_INDEX_H
#define TENACITAS_LIB_STA_TUPLE_INDEX_H

#include <cstdint>

#include <tenacitas.lib/sta/internal/type_index.h>
#include <tenacitas.lib/sta/tuple_like.h>

namespace tenacitas::lib::sta {

template <typename t_type, tuple_like t_types> struct tuple_index {
  static constexpr int16_t value = internal::type_index<0, t_type, t_types>();
};

template <typename t_type, tuple_like t_types>
inline constexpr int16_t tuple_index_v = tuple_index<t_type, t_types>::value;

} // namespace tenacitas::lib::sta
#endif
