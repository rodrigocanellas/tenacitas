/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_SUBSCRIBER_H
#define TNCT_TRAITS_ASYNC_SUBSCRIBER_H

#include <tuple>

#include "tnct/traits/async/event.h"
#include "tnct/traits/tuple/contains_type.h"
#include "tnct/traits/tuple/like.h"

namespace tnct::traits::async {

template <typename t, typename t_event>
concept handler_operator = requires(t p_t) {
  requires event<t_event>;

  { p_t.operator()(std::declval<t_event &&>()) } -> std::same_as<void>;
};

template <typename t, typename t_event>
concept handler = requires(t p_t) {
  typename t::events_handled;

  requires

      event<t_event> &&

      tuple::like<typename t::events_handled> &&

      tuple::contains_type<typename t::events_handled, t_event>() &&

          // every element of t::events_subscribed is a traits::event and there
          // is a 'handle' method for every 'event' in t::events_subscribed

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (

        (event<typename std::tuple_element_t<t_idx,
                                             typename t::events_handled>> &&
         handler_operator<t, typename std::tuple_element_t<
                                 t_idx, typename t::events_handled>>)

        &&...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events_handled>>());
};

} // namespace tnct::traits::async

#endif
