/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_TRAITS_IS_DISPATCHER_H
#define TENACITAS_SRC_ASYNC_TRAITS_IS_DISPATCHER_H

// #include <tuple>

#include "tenacitas/src/async/traits/tuple_contains_only_events.h"
#include "tenacitas/src/traits/has_new_operator.h"
#include "tenacitas/src/traits/tuple/contains_tuple.h"
#include "tenacitas/src/traits/tuple/is_tuple.h"
#include "tenacitas/src/traits/tuple/size_greather_or_equal.h"

namespace tenacitas::src::async::traits
{

template <typename t, typename... t_events>
concept is_dispatcher =
    requires { typename t::events; }

    && not std::copy_constructible<t>

    && not std::move_constructible<t>

    && not src::traits::has_new_operator_v<t>

    && not std::assignable_from<t &, t>

    && src::traits::tuple::is_tuple<typename t::events>

    && src::traits::tuple::is_tuple<std::tuple<t_events...>>

    && src::traits::tuple_contains_only_events<typename t::events>

    && src::traits::tuple_contains_only_events<std::tuple<t_events...>>

    && src::traits::tuple::size_greather_or_equal<typename t::events,
                                                  std::tuple<t_events...>>

    && src::traits::tuple::contains_tuple<typename t::events,
                                          std::tuple<t_events...>>

    ;

} // namespace tenacitas::src::async::traits

#endif
