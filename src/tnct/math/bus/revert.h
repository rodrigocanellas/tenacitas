#ifndef TNCT_MATH_BUS_REVERT_H
#define TNCT_MATH_BUS_REVERT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <type_traits>

namespace tnct::math::bus
{
template <std::signed_integral t_type>
constexpr std::make_unsigned_t<t_type> revert(t_type p_value)
{
  //  Two's complement safe absolute conversion avoiding overflow at INT_MIN
  using unsigned_type = std::make_unsigned_t<t_type>;
  return static_cast<unsigned_type>(-(p_value + 1))
         + static_cast<unsigned_type>(1);
}

template <typename>
inline constexpr bool always_false_v = false;

template <std::unsigned_integral t_type>
constexpr std::make_signed_t<t_type> revert(t_type p_value);

} // namespace tnct::math::bus

#endif
