/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_HANDLING_H
#define TNCT_TRAITS_HANDLING_H

#include <type_traits>

#include "tnct/traits/async/event.h"
#include "tnct/traits/container/queue.h"
#include "tnct/traits/log/logger.h"

#include "tnct/traits/tuple/like.h"

namespace tnct::traits::async {

template <typename t>
concept handling = requires(t p_t) {
  typename t::event;
  typename t::queue;
  typename t::logger;
  typename t::handler;

  async::event<typename t::event>;
  container::queue<typename t::queue, typename t::event>;
  log::logger<typename t::logger>;
};

template <typename t_event, typename t_handlings_tuple>
concept all_handlings_are_for_same_event = requires {
  requires tuple::like<t_handlings_tuple> &&

      async::event<t_event> &&

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (

        (std::is_same_v<t_event, typename std::tuple_element_t<
                                     t_idx, t_handlings_tuple>::event>)

        &&...);
  }
  (std::make_index_sequence<std::tuple_size_v<t_handlings_tuple>>());
};

} // namespace tnct::traits::async

#endif
