/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HAS_EVENTS_HANDLED_H
#define TENACITAS_SRC_TRAITS_ASYNC_HAS_EVENTS_HANDLED_H

#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/tuple/contains_type.h"

namespace tenacitas::src::traits::async {

template <typename t, typename t_event>
concept has_events_handled =

    requires {
  typename t::events_handled;
}

&&

    traits::async::event<t_event> &&

        traits::tuple::contains_type<typename t::events_handled, t_event>;

} // namespace tenacitas::src::traits::async

#endif
