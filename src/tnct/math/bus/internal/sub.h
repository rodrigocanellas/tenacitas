#ifndef TNCT_MATH_BUS_INTERNAL_SUB_H
#define TNCT_MATH_BUS_INTERNAL_SUB_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#include "tnct/math/bus/internal/fits.h"
#include "tnct/math/bus/internal/sum.h"
#include "tnct/math/bus/revert.h"

namespace tnct::math::bus::internal
{

template <std::signed_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> sub_same(t_number p_first,
                                                         t_number p_second)
{
  if ((p_second > 0)
      && (p_first < (std::numeric_limits<t_number>::min() + p_second)))
  {
    return std::nullopt;
  }

  if ((p_second < 0)
      && (p_first > (std::numeric_limits<t_number>::max() + p_second)))
  {
    return std::nullopt;
  }

  return {static_cast<t_number>(p_first - p_second)};
}

template <std::unsigned_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> sub_same(t_number p_first,
                                                         t_number p_second)
{
  if (p_first < p_second)
  {
    return std::nullopt;
  }

  return {static_cast<t_number>(p_first - p_second)};
}

template <std::integral t_result, std::integral t_first, std::integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sub_same_operands(t_first p_first, t_second p_second)
{
  using common_type = std::common_type_t<t_first, t_second>;

  const common_type _first{static_cast<common_type>(p_first)};
  const common_type _second{static_cast<common_type>(p_second)};

  const auto _maybe{internal::sub_same(_first, _second)};
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

template <std::signed_integral t_result, std::signed_integral t_first,
          std::unsigned_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sub_different_operands_signed_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<std::make_unsigned_t<t_first>, t_second>;

  if (p_first >= 0)
  {
    // p_first - p_second
    // with p_first >= 0 and p_second >= 0
    //
    // Possible outcomes:
    //
    //   p_first == p_second  -> result = 0
    //   p_first >  p_second  -> result = +(p_first - p_second)
    //   p_first <  p_second  -> result = -(p_second - p_first)

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    if (_first == _second)
    {
      return t_result{0};
    }
    else if (_first > _second)
    {
      const unsigned_common _positive_diff{
          static_cast<unsigned_common>(_first - _second)};

      if (internal::fits<t_result>(_positive_diff))
      {
        return static_cast<t_result>(_positive_diff);
      }
    }
    else
    {
      const unsigned_common _negative_magnitude{
          static_cast<unsigned_common>(_second - _first)};

      using signed_common = std::make_signed_t<unsigned_common>;

      if (internal::fits<signed_common>(_negative_magnitude))
      {
        const signed_common _negative_value{static_cast<signed_common>(
            -static_cast<signed_common>(_negative_magnitude))};

        if (internal::fits<t_result>(_negative_value))
        {
          return static_cast<t_result>(_negative_value);
        }
      }
    }
  }
  else
  {
    // p_first - p_second
    // with p_first < 0 and p_second >= 0
    //
    // Let:
    //   a = |p_first|
    //
    // Then:
    //
    //   p_first - p_second = -(a + p_second)
    //
    // The result is always negative.
    // Its magnitude is (a + p_second).

    const unsigned_common _first_magnitude{
        static_cast<unsigned_common>(revert(p_first))};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    const auto _maybe_negative_magnitude{
        internal::sum_same_operands<unsigned_common>(_first_magnitude,
                                                     _second)};

    if (_maybe_negative_magnitude)
    {
      using signed_common = std::make_signed_t<unsigned_common>;

      if (internal::fits<signed_common>(*_maybe_negative_magnitude))
      {
        const signed_common _negative_value{static_cast<signed_common>(
            -static_cast<signed_common>(*_maybe_negative_magnitude))};

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
sub_different_operands_signed_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<t_first, std::make_unsigned_t<t_second>>;

  if (p_second >= 0)
  {
    // p_first - p_second
    // with p_first >= 0 and p_second >= 0
    //
    // Possible outcomes:
    //
    //   p_first == p_second  -> result = 0
    //   p_first >  p_second  -> result = +(p_first - p_second)
    //   p_first <  p_second  -> result = -(p_second - p_first)

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    if (_first == _second)
    {
      return t_result{0};
    }
    else if (_first > _second)
    {
      const unsigned_common _positive_diff{
          static_cast<unsigned_common>(_first - _second)};

      if (internal::fits<t_result>(_positive_diff))
      {
        return static_cast<t_result>(_positive_diff);
      }
    }
    else
    {
      const unsigned_common _negative_magnitude{
          static_cast<unsigned_common>(_second - _first)};

      using signed_common = std::make_signed_t<unsigned_common>;

      if (internal::fits<signed_common>(_negative_magnitude))
      {
        const signed_common _negative_value{static_cast<signed_common>(
            -static_cast<signed_common>(_negative_magnitude))};

        if (internal::fits<t_result>(_negative_value))
        {
          return static_cast<t_result>(_negative_value);
        }
      }
    }
  }
  else
  {
    // p_first - p_second
    // with p_first >= 0 and p_second < 0
    //
    // Let:
    //   a = |p_second|
    //
    // Then:
    //
    //   p_first - p_second = p_first + a
    //
    // The result is always non-negative.
    // We compute it as an addition of magnitudes.

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second_magnitude{
        static_cast<unsigned_common>(revert(p_second))};

    const auto _maybe_positive{internal::sum_same_operands<unsigned_common>(
        _first, _second_magnitude)};

    if (_maybe_positive && internal::fits<t_result>(*_maybe_positive))
    {
      return static_cast<t_result>(*_maybe_positive);
    }
  }

  return std::nullopt;
}

template <std::unsigned_integral t_result, std::signed_integral t_first,
          std::unsigned_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sub_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<std::make_unsigned_t<t_first>, t_second>;

  if (p_first >= 0)
  {
    // p_first - p_second
    // with p_first >= 0 and p_second >= 0
    //
    // The result can only be represented in an unsigned type if:
    //
    //   p_first >= p_second
    //
    // Otherwise the mathematical result would be negative,
    // which cannot be represented in t_result (unsigned).

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    const auto _maybe{internal::sub_same(_first, _second)};

    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }
  else
  {
    // p_first - p_second
    // with p_first < 0 and p_second >= 0
    //
    // Let:
    //   a = |p_first|
    //
    // Then:
    //
    //   p_first - p_second = -(a + p_second)
    //
    // The result is always negative and therefore cannot be
    // represented in an unsigned result type.

    return std::nullopt;
  }

  return std::nullopt;
}

template <std::unsigned_integral t_result, std::unsigned_integral t_first,
          std::signed_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sub_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<t_first, std::make_unsigned_t<t_second>>;

  if (p_second >= 0)
  {
    // p_first - p_second
    // with p_first >= 0 and p_second >= 0
    //
    // This reduces to unsigned - unsigned.
    // If p_first < p_second, the mathematical result is negative
    // and cannot be represented in an unsigned result type.

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    const auto _maybe{internal::sub_same(_first, _second)};

    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }
  else
  {
    // p_first - p_second
    // with p_first >= 0 and p_second < 0
    //
    // Let:
    //   a = |p_second|
    //
    // Then:
    //
    //   p_first - p_second = p_first + a
    //
    // The result is always non-negative.
    // We compute it as an addition of magnitudes.

    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second_magnitude{
        static_cast<unsigned_common>(revert(p_second))};

    const auto _maybe{internal::sum_same_operands<unsigned_common>(
        _first, _second_magnitude)};

    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }

  return std::nullopt;
}

} // namespace tnct::math::bus::internal

#endif
