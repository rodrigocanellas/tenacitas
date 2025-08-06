/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_TRAITS_CONTAINS_ONLY_EVENTS_H
#define TENACITAS_SRC_ASYNC_TRAITS_CONTAINS_ONLY_EVENTS_H

#include <tuple>

#include "tenacitas/src/async/traits/is_event.h"
#include "tenacitas/src/tuple/traits/is_tuple.h"

namespace tenacitas::src::async::traits
{
template <typename t_tuple>
concept tuple_contains_only_events =

    src::tuple::traits::is_tuple<t_tuple> &&

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>)
{
  return (src::async::traits::is_event<std::tuple_element_t<t_idx, t_tuple>>
          && ...);
}(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

} // namespace tenacitas::src::async::traits
#endif
