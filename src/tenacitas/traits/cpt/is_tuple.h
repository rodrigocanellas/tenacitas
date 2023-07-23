#ifndef TENACITAS_LIB_TRAITS_CPT_IS_TUPLE_H
#define TENACITAS_LIB_TRAITS_CPT_IS_TUPLE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <tuple>

/// \brief master namespace
namespace tenacitas::lib::traits::cpt {

// \file Solution based on
// https://stackoverflow.com/questions/68443804/c20-concept-to-check-tuple-like-types

template <typename t_tuple, std::size_t t_idx>
concept has_tuple_element = requires(t_tuple t) {
  typename std::tuple_element_t<t_idx, std::remove_const_t<t_tuple>>;
  {
    get<t_idx>(t)
    } -> std::convertible_to<const std::tuple_element_t<t_idx, t_tuple> &>;
};

template <typename t_tuple>
concept tuple_like = !std::is_reference_v<t_tuple> && requires(t_tuple t) {
  typename std::tuple_size<t_tuple>::type;
  requires std::derived_from<
      std::tuple_size<t_tuple>,
      std::integral_constant<std::size_t, std::tuple_size_v<t_tuple>>>;
}
&&[]<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (has_tuple_element<t_tuple, t_idx> && ...);
}
(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

} // namespace tenacitas::lib::traits::cpt

#endif
