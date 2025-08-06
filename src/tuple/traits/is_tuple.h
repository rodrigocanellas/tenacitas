/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TUPLE_TRAITS_IS_TUPLE_H
#define TENACITAS_SRC_TUPLE_TRAITS_IS_TUPLE_H

#include "tenacitas/src/tuple/traits/is.h"

namespace tenacitas::src::tuple::traits
{

template <typename t_tuple>
concept is_tuple = src::tuple::traits::is_v<t_tuple>;

} // namespace tenacitas::src::tuple::traits
#endif
