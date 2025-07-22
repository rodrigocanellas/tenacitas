/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H
#define TENACITAS_SRC_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H

#include <tuple>

#include "tenacitas/src/traits/tuple/like.h"

namespace tenacitas::src::traits::tuple {

template <typename t_tuple_1, typename t_tuple_2>
concept size_greather_or_equal =

    like<t_tuple_1> &&

    like<t_tuple_2> &&

    (std::tuple_size_v<t_tuple_1> >= std::tuple_size_v<t_tuple_2>);

} // namespace tenacitas::src::traits::tuple
#endif
