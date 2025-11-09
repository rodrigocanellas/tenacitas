/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_CPT_HAS_EVENTS_PUBLISHED_H
#define TNCT_ASYNC_CPT_HAS_EVENTS_PUBLISHED_H

#include "tnct/async/cpt/is_event.h"
#include "tnct/tuple/bus/contains_type.h"

namespace tnct::async::cpt
{

template <typename t, typename t_event>
concept has_events_published =

    requires { typename t::events_published; }

    &&

    async::cpt::is_event<t_event> &&

    tuple::bus::contains_type<typename t::events_published, t_event>;

} // namespace tnct::async::cpt

#endif
