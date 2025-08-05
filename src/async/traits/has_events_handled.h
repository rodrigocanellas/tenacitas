/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_TRAITS_HAS_EVENTS_HANDLED_H
#define TENACITAS_SRC_ASYNC_TRAITS_HAS_EVENTS_HANDLED_H

#include "tenacitas/src/async/traits/is_event.h"
#include "tenacitas/src/traits/tuple/contains_type.h"

namespace tenacitas::src::async::traits {

template <typename t, typename t_event>
concept has_events_handled =

    requires {
  typename t::events_handled;
}

&&

    src::async::traits::event<t_event> &&

        traits::tuple::contains_type<typename t::events_handled, t_event>;

} // namespace tenacitas::src::async::traits

#endif
