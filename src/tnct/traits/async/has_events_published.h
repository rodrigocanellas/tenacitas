/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HAS_EVENTS_PUBLISHED_H
#define TNCT_TRAITS_ASYNC_HAS_EVENTS_PUBLISHED_H

#include "tnct/traits/async/event.h"
#include "tnct/traits/tuple/contains_type.h"

namespace tnct::traits::async {

template <typename t, typename t_event>
concept has_events_published =

    requires {
  typename t::events_published;
}

&&

    traits::async::event<t_event> &&

        traits::tuple::contains_type<typename t::events_published, t_event>;

} // namespace tnct::traits::async

#endif
