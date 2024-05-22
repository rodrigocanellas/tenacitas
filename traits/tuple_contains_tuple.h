/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TUPLE_CONTAINS_TUPLE_H
#define TENACITAS_LIB_TRAITS_TUPLE_CONTAINS_TUPLE_H

// #include <tuple>

#include <tenacitas.lib/traits/is_smart_ptr.h>
#include <tenacitas.lib/traits/is_tuple.h>
#include <tenacitas.lib/traits/is_type_in_tuple.h>
#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits {
template <typename t_container_tuple, typename t_contained_tuple>
concept tuple_contains_tuple =

    is_tuple_v<t_container_tuple> &&

    is_tuple_v<t_contained_tuple> &&

        []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (is_type_in_tuple_v<std::tuple_element_t<t_idx, t_contained_tuple>,
                             t_container_tuple> &&
          ...);
}
(std::make_index_sequence<std::tuple_size_v<t_contained_tuple>>());
;

} // namespace tenacitas::lib::traits

#endif
