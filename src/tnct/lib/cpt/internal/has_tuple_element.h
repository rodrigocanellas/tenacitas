/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_CPT_INTERNAL_HAS_TUPLE_ELEMENT_H
#define TNCT_LIB_CPT_INTERNAL_HAS_TUPLE_ELEMENT_H

#include <tuple>

namespace tnct::lib::cpt::internal {

template <typename t_tuple, std::size_t t_idx>
concept has_tuple_element = requires(t_tuple t) {
  typename std::tuple_element_t<t_idx, std::remove_const_t<t_tuple>>;
  {
    get<t_idx>(t)
    } -> std::convertible_to<const std::tuple_element_t<t_idx, t_tuple> &>;
};

} // namespace tnct::lib::cpt::internal

namespace tlc = tnct::lib::cpt;

#endif
