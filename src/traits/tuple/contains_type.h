/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_TUPLE_CONTAINS_TYPE_H
#define TENACITAS_SRC_TRAITS_TUPLE_CONTAINS_TYPE_H

#include "tenacitas/src/traits/tuple/find.h"
#include "tenacitas/src/traits/tuple/is_tuple.h"

namespace tenacitas::src::traits::tuple {

// code reviewed by ChatGPT from a previous version I wrote
template <traits::tuple::is_tuple t_tuple, typename t_type>
constexpr bool contains_type() {
  return find<t_tuple, t_type>().has_value();
}

} // namespace tenacitas::src::traits::tuple

#endif
