/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_SUBSCRIBER_H
#define TENACITAS_LIB_TRAITS_SUBSCRIBER_H

#include <tuple>

#include <tenacitas.lib/traits/event.h>
#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits {

template <typename t, typename t_event>
concept has_handle_method = requires(t p_t) {
  requires event<t_event>;

  { p_t.handle(std::declval<t_event &&>()) } -> std::same_as<void>;
};

template <typename t>
concept subscriber = requires(t p_t) {
  typename t::events_subscribed;

  requires tuple_like<typename t::events_subscribed>

      && // every element of t::events_subscribed is a traits::event and there
         // is a 'handle' method for every 'event' in t::events_subscribed

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (
        (event<typename std::tuple_element_t<t_idx,
                                             typename t::events_subscribed>> &&
         (has_handle_method<t, typename std::tuple_element_t<
                                   t_idx, typename t::events_subscribed>>)

             )

        && ...);
  }
  (std::make_index_sequence<
      std::tuple_size_v<typename t::events_subscribed>>());
};

} // namespace tenacitas::lib::traits

#endif
