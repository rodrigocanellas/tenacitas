/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TRAITS_HAS_EVENTS_HANDLED_H
#define TNCT_ASYNC_TRAITS_HAS_EVENTS_HANDLED_H

#include "tnct/async/traits/is_event.h"
#include "tnct/tuple/contains_type.h"

namespace tnct::async::traits {

template <typename t, typename t_event>
concept has_events_handled =

    requires {
  typename t::events_handled;
}

&&

    async::traits::event<t_event> &&

        traits::tuple::contains_type<typename t::events_handled, t_event>;

} // namespace tnct::async::traits

#endif
