/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_CONTAINS_TYPE_H
#define TNCT_TRAITS_TUPLE_CONTAINS_TYPE_H

#include "tnct/traits/tuple/find.h"
#include "tnct/traits/tuple/like.h"

namespace tnct::traits::tuple {

// code reviewed by ChatGPT from a previous version I wrote
template <traits::tuple::like t_tuple, typename t_type>
constexpr bool contains_type() {
  return find<t_tuple, t_type>().has_value();
}

} // namespace tnct::traits::tuple

#endif
