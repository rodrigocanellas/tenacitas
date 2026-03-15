#ifndef TNCT_MATH_BUS_INTERNAL_FITS_H
#define TNCT_MATH_BUS_INTERNAL_FITS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

namespace tnct::math::bus::internal
{

template <std::signed_integral t_result, std::signed_integral t_number>
[[nodiscard]] constexpr bool fits(t_number p_number)
{
  using common_type = std::common_type_t<t_result, t_number>;

  const common_type _number{static_cast<common_type>(p_number)};

  return (_number
          >= static_cast<common_type>(std::numeric_limits<t_result>::min()))
         && (_number
             <= static_cast<common_type>(std::numeric_limits<t_result>::max()));
}

template <std::unsigned_integral t_result, std::unsigned_integral t_number>
[[nodiscard]] constexpr bool fits(t_number p_number)
{
  using common_type = std::common_type_t<t_result, t_number>;

  return static_cast<common_type>(p_number)
         <= static_cast<common_type>(std::numeric_limits<t_result>::max());
}

template <std::unsigned_integral t_result, std::signed_integral t_number>
[[nodiscard]] constexpr bool fits(t_number p_number)
{
  if (p_number < 0)
  {
    return false;
  }

  using unsigned_number = std::make_unsigned_t<t_number>;
  using common_type     = std::common_type_t<t_result, unsigned_number>;

  return static_cast<common_type>(static_cast<unsigned_number>(p_number))
         <= static_cast<common_type>(std::numeric_limits<t_result>::max());
}

template <std::signed_integral t_result, std::unsigned_integral t_number>
[[nodiscard]] constexpr bool fits(t_number p_number)
{
  using common_type =
      std::common_type_t<t_number, std::make_unsigned_t<t_result>>;

  return static_cast<common_type>(p_number)
         <= static_cast<common_type>(std::numeric_limits<t_result>::max());
}

} // namespace tnct::math::bus::internal

#endif
