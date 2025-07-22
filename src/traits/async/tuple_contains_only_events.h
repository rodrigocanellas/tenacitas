/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_TUPLE_CONTAINS_ONLY_EVENTS_H
#define TENACITAS_SRC_TRAITS_TUPLE_CONTAINS_ONLY_EVENTS_H

#include <tuple>

#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/tuple/like.h"

namespace tenacitas::src::traits {
template <typename t_tuple>
concept tuple_contains_only_events =

    tuple::like<t_tuple> &&

        []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (async::event<std::tuple_element_t<t_idx, t_tuple>> && ...);
}
(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

} // namespace tenacitas::src::traits
#endif
