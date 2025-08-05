/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_TUPLE_IS_TUPLE_H
#define TENACITAS_SRC_TRAITS_TUPLE_IS_TUPLE_H

#include "tenacitas/src/traits/tuple/is.h"

namespace tenacitas::src::traits::tuple
{

template <typename t_tuple>
concept is_tuple = traits::tuple::is_v<t_tuple>;

} // namespace tenacitas::src::traits::tuple
#endif
