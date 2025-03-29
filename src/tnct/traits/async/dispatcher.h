/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_DISPATCHER_H
#define TNCT_TRAITS_ASYNC_DISPATCHER_H

// #include <tuple>

#include "tnct/traits/async/tuple_contains_only_events.h"
#include "tnct/traits/has_new_operator.h"
// #include "tnct/traits/tuple/contains_tuple.h"
#include "tnct/traits/tuple/like.h"
#include "tnct/traits/tuple/size_greather_or_equal.h"

namespace tnct::traits::async {

template <typename t, typename... t_events>
concept dispatcher = requires {
  typename t::events;
}

&&!std::copy_constructible<t>

    && not std::copy_constructible<t>

    && not std::move_constructible<t>

    && not has_new_operator_v<t>

    && not std::assignable_from<t &, t>

    && tuple::like<typename t::events>

    && tuple::like<std::tuple<t_events...>>

    && tuple_contains_only_events<typename t::events>

    && tuple_contains_only_events<std::tuple<t_events...>>

    &&
    tuple::size_greather_or_equal<typename t::events, std::tuple<t_events...>>

    // && tuple::contains_tuple<typename t::events, std::tuple<t_events...>>

    ;

} // namespace tnct::traits::async

#endif
