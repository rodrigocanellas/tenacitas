/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_CPT_TUPLE_CONTAINS_ONLY_EVENTS_H
#define TNCT_ASYNC_CPT_TUPLE_CONTAINS_ONLY_EVENTS_H

#include <tuple>

#include "tnct/async/cpt/is_event.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::async::cpt
{
template <typename t_tuple>
concept tuple_contains_only_events =

    tuple::cpt::is_tuple<t_tuple> &&

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>)
{
  return (async::cpt::is_event<std::tuple_element_t<t_idx, t_tuple>> && ...);
}(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

} // namespace tnct::async::cpt
#endif
