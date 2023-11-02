#ifndef TENACITAS_LIB_ASYNC_CPT_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_CPT_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <memory>

#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/async/typ/priority.h>
#include <tenacitas.lib/src/async/typ/queue_id.h>
#include <tenacitas.lib/src/traits/alg/tuple.h>
#include <tenacitas.lib/src/traits/cpt/is_tuple.h>

namespace tenacitas::lib::async::cpt {

template <typename t, typename... t_events>
concept dispatcher = requires(t p_t) {
  typename t::events;

  traits::cpt::tuple_like<typename t::events>

      && // every type in t::events is a cpt::event

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (cpt::event<std::tuple_element<t_idx, typename t::events>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::event>>())

      && // every type in t_events is a cpt::event

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (cpt::event<std::tuple_element<t_idx, std::tuple<t_events...>>> &&
            ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>())

      && // every event in t_events exists in t::events

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {

    return (

        traits::alg::idx<0,
                         std::tuple_element_t<t_idx, std::tuple<t_events...>>,
                         typename t::events> &&

        ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>());
};

} // namespace tenacitas::lib::async::cpt

#endif
