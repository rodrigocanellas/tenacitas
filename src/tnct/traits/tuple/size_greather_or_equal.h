/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H
#define TNCT_TRAITS_TUPLE_SIZE_GREATER_OR_EQUAL_H

#include <tuple>

#include "tnct/traits/tuple/like.h"

namespace tnct::traits::tuple {

template <typename t_tuple_1, typename t_tuple_2>
concept size_greather_or_equal =

    like<t_tuple_1> &&

    like<t_tuple_2> &&

    (std::tuple_size_v<t_tuple_1> >= std::tuple_size_v<t_tuple_2>);

} // namespace tnct::traits::tuple
#endif
