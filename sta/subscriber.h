/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_SUBSCRIBER_H
#define TENACITAS_LIB_STA_SUBSCRIBER_H

#include <tuple>

#include <tenacitas.lib/sta/event.h>
#include <tenacitas.lib/sta/tuple_like.h>

namespace tenacitas::lib::sta {

template <typename t>
concept subscriber = requires(t p_t) {
  typename t::events_subscribed;

  requires tuple_like<typename t::events_subscribed> &&

      []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (
        (event<typename std::tuple_element_t<t_idx,
                                             typename t::events_subscribed>>)

        &&...);
  }
  (std::make_index_sequence<
      std::tuple_size_v<typename t::events_subscribed>>());
};

} // namespace tenacitas::lib::sta

#endif
