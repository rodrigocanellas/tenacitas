/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TUPLE_LIKE_H
#define TENACITAS_LIB_TRAITS_TUPLE_LIKE_H

#include "tenacitas.lib/traits/is_tuple.h"

namespace tenacitas::lib::traits {

template <typename t_tuple>
concept tuple_like = traits::is_tuple_v<t_tuple>;

} // namespace tenacitas::lib::traits
#endif
