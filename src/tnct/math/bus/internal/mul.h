#ifndef TNCT_MATH_BUS_INTERNAL_MUL_H
#define TNCT_MATH_BUS_INTERNAL_MUL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#include "tnct/math/bus/internal/fits.h"
#include "tnct/math/bus/revert.h"

namespace tnct::math::bus::internal
{

template <std::signed_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> mul_same(t_number p_first,
                                                         t_number p_second)
{
  if ((p_first == 0) || (p_second == 0))
  {
    return t_number{0};
  }

  if ((p_first == std::numeric_limits<t_number>::min())
      && (p_second == t_number{-1}))
  {
    return std::nullopt;
  }

  if ((p_second == std::numeric_limits<t_number>::min())
      && (p_first == t_number{-1}))
  {
    return std::nullopt;
  }

  if (p_first > 0)
  {
    if (p_second > 0)
    {
      if (p_first > (std::numeric_limits<t_number>::max() / p_second))
      {
        return std::nullopt;
      }
    }
    else
    {
      if (p_second < (std::numeric_limits<t_number>::min() / p_first))
      {
        return std::nullopt;
      }
    }
  }
  else
  {
    if (p_second > 0)
    {
      if (p_first < (std::numeric_limits<t_number>::min() / p_second))
      {
        return std::nullopt;
      }
    }
    else
    {
      if (p_first < (std::numeric_limits<t_number>::max() / p_second))
      {
        return std::nullopt;
      }
    }
  }

  return static_cast<t_number>(p_first * p_second);
}

template <std::unsigned_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> mul_same(t_number p_first,
                                                         t_number p_second)
{
  if ((p_first == 0) || (p_second == 0))
  {
    return t_number{0};
  }

  if (p_first > (std::numeric_limits<t_number>::max() / p_second))
  {
    return std::nullopt;
  }

  return static_cast<t_number>(p_first * p_second);
}

template <std::integral t_result, std::integral t_first, std::integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
mul_same_operands(t_first p_first, t_second p_second)
{
  using common_type = std::common_type_t<t_first, t_second>;

  const common_type _first{static_cast<common_type>(p_first)};
  const common_type _second{static_cast<common_type>(p_second)};

  const auto _maybe{internal::mul_same(_first, _second)};
  if (!_maybe)
  {
    return std::nullopt;
  }

  if (!internal::fits<t_result>(*_maybe))
  {
    return std::nullopt;
  }

  return static_cast<t_result>(*_maybe);
}

template <std::unsigned_integral t_result, std::signed_integral t_first,
          std::unsigned_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
mul_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<std::make_unsigned_t<t_first>, t_second>;

  if (p_first == 0)
  {
    return t_result{0};
  }

  if (p_second == 0)
  {
    return t_result{0};
  }

  if (p_first < 0)
  {
    return std::nullopt;
  }

  const unsigned_common _first{static_cast<unsigned_common>(p_first)};
  const unsigned_common _second{static_cast<unsigned_common>(p_second)};

  const auto _maybe{
      internal::mul_same_operands<unsigned_common>(_first, _second)};

  if (_maybe && internal::fits<t_result>(*_maybe))
  {
    return static_cast<t_result>(*_maybe);
  }

  return std::nullopt;
}

template <std::unsigned_integral t_result, std::unsigned_integral t_first,
          std::signed_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
mul_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  return mul_different_operands_unsigned_result<t_result>(p_second, p_first);
}

template <std::signed_integral t_result, std::signed_integral t_first,
          std::unsigned_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
mul_different_operands_signed_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<std::make_unsigned_t<t_first>, t_second>;

  if ((p_first == 0) || (p_second == 0))
  {
    return t_result{0};
  }

  if (p_first > 0)
  {
    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    const auto _maybe{
        internal::mul_same_operands<unsigned_common>(_first, _second)};

    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }
  else
  {
    const unsigned_common _first_magnitude{
        static_cast<unsigned_common>(revert(p_first))};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    const auto _maybe_magnitude{internal::mul_same_operands<unsigned_common>(
        _first_magnitude, _second)};

    if (_maybe_magnitude)
    {
      using signed_common = std::make_signed_t<unsigned_common>;

      if (internal::fits<signed_common>(*_maybe_magnitude))
      {
        const signed_common _negative_value{static_cast<signed_common>(
            -static_cast<signed_common>(*_maybe_magnitude))};

        if (internal::fits<t_result>(_negative_value))
        {
          return static_cast<t_result>(_negative_value);
        }
      }
    }
  }

  return std::nullopt;
}

template <std::signed_integral t_result, std::unsigned_integral t_first,
          std::signed_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
mul_different_operands_signed_result(t_first p_first, t_second p_second)
{
  return mul_different_operands_signed_result<t_result>(p_second, p_first);
}

} // namespace tnct::math::bus::internal

#endif
