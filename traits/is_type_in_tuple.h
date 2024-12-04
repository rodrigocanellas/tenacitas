/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_IS_TYPE_IN_TUPLE_H
#define TENACITAS_LIB_TRAITS_IS_TYPE_IN_TUPLE_H

#include "tenacitas.lib/traits/tuple_find.h"
#include "tenacitas.lib/traits/tuple_like.h"

namespace tenacitas::lib::traits {

template <traits::tuple_like t_tuple, typename t_type>
static constexpr bool
    is_type_in_tuple = traits::tuple_find<t_tuple, t_type>() != -1;

} // namespace tenacitas::lib::traits

#endif
