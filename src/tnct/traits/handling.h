/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_HANDLING_H
#define TNCT_TRAITS_HANDLING_H

#include <type_traits>

#include "tnct/traits/event.h"
#include "tnct/traits/logger.h"
#include "tnct/traits/queue.h"

#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

template <typename t>
concept handling = requires(t p_t) {
  typename t::event;
  typename t::queue;
  typename t::logger;
  typename t::handler;

  event<typename t::event>;
  queue<typename t::queue, typename t::event>;
  logger<typename t::logger>;
};

template <typename t_event, typename t_handlings_tuple>
concept all_handlings_are_for_same_event = requires {
  requires tuple_like<t_handlings_tuple> &&

      event<t_event> &&

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (

        (std::is_same_v<t_event, typename std::tuple_element_t<
                                     t_idx, t_handlings_tuple>::event>)

        &&...);
  }
  (std::make_index_sequence<std::tuple_size_v<t_handlings_tuple>>());
};

} // namespace tnct::traits

#endif
