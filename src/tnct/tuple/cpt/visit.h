/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CPT_VISIT_H
#define TNCT_TUPLE_CPT_VISIT_H

#include <cstddef>
#include <type_traits>

#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::tuple::cpt
{

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_constant_tuple =
    (tuple::cpt::is_tuple<std::remove_cvref_t<t_tuple>>)&&

    requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>(
            std::declval<const std::remove_cvref_t<t_tuple> &>())
      } -> std::same_as<bool>;
    };

template <typename t, typename t_tuple, std::size_t t_idx>
concept visit_type_in_tuple =
    (tuple::cpt::is_tuple<std::remove_cvref_t<t_tuple>>)&&

    requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>()
      } -> std::same_as<bool>;
    };

template <typename t, typename t_tuple, size_t t_idx = 0>
concept visit_value_in_mutable_tuple =
    (tuple::cpt::is_tuple<std::remove_cvref_t<t_tuple>>)&&

    requires(t p_t) {
      {
        p_t.template operator()<t_tuple, t_idx>(
            std::declval<std::remove_cvref_t<t_tuple> &>())
      } -> std::same_as<bool>;
    };

} // namespace tnct::tuple::cpt

#endif
