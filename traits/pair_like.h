/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_PAIR_LIKE_H
#define TENACITAS_LIB_TRAITS_PAIR_LIKE_H

#include <concepts>

namespace tenacitas::lib::traits {

template <typename t>
concept pair_like = requires(t p_t) {
  typename t::first_type;
  typename t::second_type;
  { p_t.first } -> std::same_as<typename t::first_type>;
  { p_t.second } -> std::same_as<typename t::second_type>;
};

} // namespace tenacitas::lib::traits
#endif
