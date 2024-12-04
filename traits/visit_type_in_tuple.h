/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_VISIT_TYPE_IN_TUPLE_H
#define TENACITAS_LIB_TRAITS_VISIT_TYPE_IN_TUPLE_H

#include <cstddef>

#include "tenacitas.lib/traits/tuple_like.h"

namespace tenacitas::lib::traits {

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_type_in_tuple = requires(t p_t) {

  requires traits::tuple_like<t_tuple>;

  { p_t.template operator()<t_tuple, t_idx>() } -> std::same_as<bool>;
};

} // namespace tenacitas::lib::traits

#endif
