/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_INTERNAL_CONTAINS_TUPLE_HELPER_H
#define TNCT_TUPLE_INTERNAL_CONTAINS_TUPLE_HELPER_H

#include <tuple>

#include "tnct/tuple/bus/contains_type.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::tuple::internal::bus
{
template <tuple::cpt::is_tuple t_container_tuple,
          tuple::cpt::is_tuple t_contained_tuple, std::size_t t_idx = 0>
constexpr bool contains_tuple_helper()
{
  if constexpr (t_idx >= std::tuple_size_v<t_contained_tuple>)
  {
    return true;
  }
  else if constexpr (!tuple::bus::contains_type<
                         t_container_tuple,
                         std::tuple_element_t<t_idx, t_contained_tuple>>())
  {
    return false;
  }
  else
  {
    return contains_tuple_helper<t_container_tuple, t_contained_tuple,
                                 t_idx + 1>(); // Recur to next index
  }
  return false;
}
} // namespace tnct::tuple::internal::bus

#endif
