/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TRAITS_IS_DISPATCHER_H
#define TNCT_ASYNC_TRAITS_IS_DISPATCHER_H

// #include <tuple>

#include "tnct/async/traits/tuple_contains_only_events.h"
#include "tnct/memory/traits/has_new_operator.h"
#include "tnct/tuple/cpt/contains_tuple.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/cpt/size_greather_or_equal.h"

namespace tnct::async::traits
{

template <typename t, typename... t_events>
concept is_dispatcher =
    requires { typename t::events; }

    && not std::copy_constructible<t>

    && not std::move_constructible<t>

    && not memory::traits::has_new_operator_v<t>

    && not std::assignable_from<t &, t>

    && tuple::cpt::is_tuple<typename t::events>

    && tuple::cpt::is_tuple<std::tuple<t_events...>>

    && async::traits::tuple_contains_only_events<typename t::events>

    && async::traits::tuple_contains_only_events<std::tuple<t_events...>>

    && tuple::cpt::size_greather_or_equal<typename t::events,
                                                  std::tuple<t_events...>>

    && tuple::cpt::contains_tuple<typename t::events,
                                          std::tuple<t_events...>>

    ;

} // namespace tnct::async::traits

#endif
