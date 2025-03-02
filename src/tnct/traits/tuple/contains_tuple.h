/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_CONTAINS_TUPLE_H
#define TNCT_TRAITS_TUPLE_CONTAINS_TUPLE_H

#include <tuple>

#include "tnct/traits/tuple/contains_type.h"
#include "tnct/traits/tuple/like.h"

namespace tnct::traits::tuple {

template <tuple::like t_container_tuple, tuple::like t_contained_tuple,
          std::size_t t_idx = 0>
constexpr bool contains_tuple_helper() {
  if constexpr (t_idx >= std::tuple_size_v<t_contained_tuple>) {
    return true;
  } else if constexpr (!contains_type<
                           t_container_tuple,
                           std::tuple_element_t<t_idx, t_contained_tuple>>()) {
    return false;
  } else {
    return contains_tuple_helper<t_container_tuple, t_contained_tuple,
                                 t_idx + 1>(); // Recur to next index
  }
}

// code reviewed by ChatGPT from a previous version I wrote
template <typename t_container_tuple, typename t_contained_tuple>
concept contains_tuple =
    (std::tuple_size_v<t_container_tuple> > 0) &&
    (std::tuple_size_v<t_contained_tuple> > 0) &&
    contains_tuple_helper<t_container_tuple, t_contained_tuple>();

} // namespace tnct::traits::tuple

#endif
