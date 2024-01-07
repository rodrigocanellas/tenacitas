/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_CPT_SUBSCRIBER_H
#define TNCT_LIB_CPT_SUBSCRIBER_H

#include <concepts>
#include <tuple>

#include <tnct/lib/cpt/event.h>
#include <tnct/lib/cpt/tuple_like.h>
#include <tnct/lib/tra/is_smart_ptr.h>
#include <tnct/lib/tra/tuple_index.h>

namespace tnct::lib::cpt {

template <typename t, typename... t_events>
concept dispatcher_c = requires(t p_t) {
  typename t::events;

  typename t::ptr;

  requires !std::copy_constructible<t> && !std::move_constructible<t> &&
      !std::default_initializable<t>;

  requires(tlt::is_smart_ptr_v<typename t::ptr> == true);

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
    return (
        tlt::tuple_index_v<std::tuple_element_t<t_idx, std::tuple<t_events...>>,
                           typename t::events> &&
        ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>());
};

} // namespace tnct::lib::cpt
namespace tlc = tnct::lib::cpt;

#endif
