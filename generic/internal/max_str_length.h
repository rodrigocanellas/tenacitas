/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_GENERIC_INTERNAL_MAX_STR_LENGHT_H
#define TENACITAS_LIB_GENERIC_INTERNAL_MAX_STR_LENGHT_H

#include <cmath>
#include <cstdint>

namespace tenacitas::lib::generic::internal {

/// \brief Maximum number of characters needed to represent a type of number
///
/// <tt>internal::max_str_length<uint16_t>()</tt> is 5, while
/// <tt>internal::max_str_length<uint32_t>()</tt> is 10
///
/// \tparam t_num_type is the type of number
template <typename t_num_type>
requires std::integral<t_num_type> || std::floating_point<t_num_type>
inline constexpr uint16_t max_str_length() {
  return (
      static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))) +
      1);
}

} // namespace tenacitas::lib::generic::internal

#endif
