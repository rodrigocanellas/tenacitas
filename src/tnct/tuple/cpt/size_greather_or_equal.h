/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CPT_SIZE_GREATER_OR_EQUAL_H
#define TNCT_TUPLE_CPT_SIZE_GREATER_OR_EQUAL_H

#include <tuple>

#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::tuple::cpt {

template <typename t_tuple_1, typename t_tuple_2>
concept size_greather_or_equal =

    is_tuple<t_tuple_1> &&

    is_tuple<t_tuple_2> &&

    (std::tuple_size_v<t_tuple_1> >= std::tuple_size_v<t_tuple_2>);

} // namespace tnct::tuple::cpt
#endif
