#ifndef TENACITAS_LIB_ASYNC_CPT_PUBLISHER_H
#define TENACITAS_LIB_ASYNC_CPT_PUBLISHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/traits/cpt/is_tuple.h>

namespace tenacitas::lib::async::cpt {

template <typename t>
concept publisher = requires(t p_t) {
  typename t::events_published;

  requires traits::cpt::tuple_like<typename t::events_published> &&

      []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return ((event<typename std::tuple_element_t<
                 t_idx, typename t::events_published>>)&&...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events_published>>());
};

} // namespace tenacitas::lib::async::cpt

#endif
