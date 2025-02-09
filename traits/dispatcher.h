/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_DISPATCHER_H
#define TENACITAS_LIB_TRAITS_DISPATCHER_H

// #include <tuple>

#include "tenacitas.lib/traits/has_new_operator.h"
#include "tenacitas.lib/traits/tuple_contains_only_events.h"
#include "tenacitas.lib/traits/tuple_contains_tuple.h"
#include "tenacitas.lib/traits/tuple_like.h"
#include "tenacitas.lib/traits/tuple_size_greather_or_equal.h"

namespace tenacitas::lib::traits {

template <typename t, typename... t_events>
concept dispatcher = requires {
  typename t::events;
}
&&!std::copy_constructible<t>

    && !std::move_constructible<t>

    && !std::is_copy_assignable_v<t>

    && !std::is_move_assignable_v<t>

    && !has_new_operator_v<t>

    && tuple_like<typename t::events>

    && tuple_like<std::tuple<t_events...>>

    && tuple_size_greather_or_equal<typename t::events, std::tuple<t_events...>>

    && tuple_contains_only_events<typename t::events>

    && tuple_contains_only_events<std::tuple<t_events...>>

    && tuple_contains_tuple<typename t::events, std::tuple<t_events...>>;

} // namespace tenacitas::lib::traits

#endif
