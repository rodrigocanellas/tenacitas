/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_PUBLISHER_H
#define TENACITAS_LIB_TRAITS_PUBLISHER_H

#include <tuple>

#include <tenacitas.lib/traits/event.h>
#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits {

template <typename t>
concept publisher = requires(t p_t) {
  typename t::events_published;

  requires tuple_like<typename t::events_published> &&

      []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return ((event<typename std::tuple_element_t<
                 t_idx, typename t::events_published>>)&&...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events_published>>());
};

} // namespace tenacitas::lib::traits

#endif
