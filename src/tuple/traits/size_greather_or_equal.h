/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TUPLE_TRAITS_SIZE_GREATER_OR_EQUAL_H
#define TENACITAS_SRC_TUPLE_TRAITS_SIZE_GREATER_OR_EQUAL_H

#include <tuple>

#include "tenacitas/src/tuple/traits/is_tuple.h"

namespace tenacitas::src::tuple::traits {

template <typename t_tuple_1, typename t_tuple_2>
concept size_greather_or_equal =

    is_tuple<t_tuple_1> &&

    is_tuple<t_tuple_2> &&

    (std::tuple_size_v<t_tuple_1> >= std::tuple_size_v<t_tuple_2>);

} // namespace tenacitas::src::tuple::traits
#endif
