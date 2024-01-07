/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_CPT_PUBLISHER_H
#define TENACITAS_LIB_CPT_PUBLISHER_H

#include <concepts>
#include <tuple>

#include <tenacitas.lib/src/cpt/event.h>
#include <tenacitas.lib/src/cpt/tuple_like.h>

namespace tenacitas::lib::cpt {

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

} // namespace tenacitas::lib::cpt
namespace tlc = tenacitas::lib::cpt;

#endif
