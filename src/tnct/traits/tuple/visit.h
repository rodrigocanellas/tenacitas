/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_VISIT_H
#define TNCT_TRAITS_TUPLE_VISIT_H

#include <cstddef>

#include "tnct/traits/tuple/like.h"

namespace tnct::traits::tuple {

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_constant_tuple = tuple::like<t_tuple> &&
    requires(t p_t) {

  {
    p_t.template operator()<t_tuple, t_idx>(std::declval<const t_tuple &>())
    } -> std::same_as<bool>;
};

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_mutable_tuple = tuple::like<t_tuple> && requires(t p_t) {
  {
    p_t.template operator()<t_tuple, t_idx>(std::declval<t_tuple &>())
    } -> std::same_as<bool>;
};

template <typename t, typename t_tuple, std::size_t t_idx>
concept visit_type_in_tuple = tuple::like<t_tuple> && requires(t p_t) {
  { p_t.template operator()<t_tuple, t_idx>() } -> std::same_as<bool>;
};

} // namespace tnct::traits::tuple

#endif
