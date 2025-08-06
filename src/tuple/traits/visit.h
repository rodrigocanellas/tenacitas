/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TUPLE_TRAITS_VISIT_H
#define TENACITAS_SRC_TUPLE_TRAITS_VISIT_H

#include <cstddef>

#include "tenacitas/src/tuple/traits/is_tuple.h"

namespace tenacitas::src::tuple::traits
{

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_constant_tuple =
    src::tuple::traits::is_tuple<t_tuple> && requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>(std::declval<const t_tuple &>())
      } -> std::same_as<bool>;
    };

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_mutable_tuple =
    src::tuple::traits::is_tuple<t_tuple> && requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>(std::declval<t_tuple &>())
      } -> std::same_as<bool>;
    };

template <typename t, typename t_tuple, std::size_t t_idx>
concept visit_type_in_tuple =
    src::tuple::traits::is_tuple<t_tuple> && requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>()
      } -> std::same_as<bool>;
    };

} // namespace tenacitas::src::tuple::traits

#endif
