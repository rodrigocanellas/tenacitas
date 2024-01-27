/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_SUBSCRIBER_H
#define TENACITAS_LIB_STA_SUBSCRIBER_H

#include <tuple>

#include <tenacitas.lib/sta/event.h>
#include <tenacitas.lib/sta/is_smart_ptr.h>
#include <tenacitas.lib/sta/tuple_index.h>
#include <tenacitas.lib/sta/tuple_like.h>

namespace tenacitas::lib::sta {

template <typename t, typename... t_events>
concept dispatcher_c = requires(t p_t) {
  typename t::events;

  typename t::ptr;

  requires !std::copy_constructible<t> && !std::move_constructible<t> &&
      !std::default_initializable<t>;

  requires(is_smart_ptr_v<typename t::ptr> == true);

  tuple_like<typename t::events> // t::events is a std::tuple

      && // every type in t::events is an event

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (event<std::tuple_element<t_idx, typename t::events>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events>>())

      && // every type in t_events is a event

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (event<std::tuple_element<t_idx, std::tuple<t_events...>>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>())

      && // every event in t_events exists in t::events

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (tuple_index_v<std::tuple_element_t<t_idx, std::tuple<t_events...>>,
                          typename t::events> &&
            ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>());
};

} // namespace tenacitas::lib::sta

#endif
