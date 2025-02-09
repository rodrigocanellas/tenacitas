/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_PUBLISHER_H
#define TNCT_TRAITS_PUBLISHER_H

#include <tuple>

#include "tnct/traits/event.h"
#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

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

} // namespace tnct::traits

#endif
