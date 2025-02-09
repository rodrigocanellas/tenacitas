/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H
#define TNCT_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H

#include <tuple>

#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

template <typename t_tuple_1, typename t_tuple_2>
concept tuple_size_greather_or_equal =

    tuple_like<t_tuple_1> &&

    tuple_like<t_tuple_2> &&

    (std::tuple_size_v<t_tuple_1> >= std::tuple_size_v<t_tuple_2>);

} // namespace tnct::traits
#endif
