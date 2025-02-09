/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_CONTAINS_TUPLE_H
#define TNCT_TRAITS_TUPLE_CONTAINS_TUPLE_H

#include <tuple>

#include "tnct/traits/is_type_in_tuple.h"
#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

template <typename t_container_tuple, typename t_contained_tuple>
concept tuple_contains_tuple =

    tuple_like<t_container_tuple> &&

    tuple_like<t_contained_tuple> &&

        []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (is_type_in_tuple<t_container_tuple,
                           std::tuple_element_t<t_idx, t_contained_tuple>> &&
          ...);
}
(std::make_index_sequence<std::tuple_size_v<t_contained_tuple>>());

} // namespace tnct::traits

#endif
