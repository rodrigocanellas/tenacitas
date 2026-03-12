#ifndef TNCT_MATH_BUS_INTERNAL_SUM_H
#define TNCT_MATH_BUS_INTERNAL_SUM_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#include "tnct/math/bus/revert.h"

#include <iostream>

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

template <std::signed_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> sum_same(t_number p_first,
                                                         t_number p_second)
{

  if (

      (

          (p_second > 0)
          && (p_first > (std::numeric_limits<t_number>::max() - p_second))

              )
      ||

      (

          (p_second < 0)
          && (p_first < (std::numeric_limits<t_number>::min() - p_second))

              )

  )

  {
    return std::nullopt;
  }
  return {p_first + p_second};
}

template <std::unsigned_integral t_number>
[[nodiscard]] constexpr std::optional<t_number> sum_same(t_number p_first,
                                                         t_number p_second)
{

  const t_number _result{static_cast<t_number>(p_first + p_second)};

  if (_result < p_first)
  {
    return std::nullopt;
  }

  return _result;
}

template <std::integral t_result, std::integral t_first, std::integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sum_same_operands(t_first p_first, t_second p_second)
{
  using common_type = std::common_type_t<t_first, t_second>;

  const common_type _first{static_cast<common_type>(p_first)};
  const common_type _second{static_cast<common_type>(p_second)};

  const auto _maybe{internal::sum_same(_first, _second)};
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
sum_different_operands_signed_result(t_first p_first, t_second p_second)
{
  if (p_first >= 0)
  {
    using unsigned_common =
        std::common_type_t<std::make_unsigned_t<t_first>, t_second>;
    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};
    const auto            _maybe{
        internal::sum_same_operands<unsigned_common>(_first, _second)};
    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }
  else
  {
    using unsigned_common =
        std::common_type_t<std::make_unsigned_t<t_first>, t_second>;
    const unsigned_common _first{static_cast<unsigned_common>(revert(p_first))};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    if (_second == _first)
    {
      return static_cast<t_result>(0);
    }
    else if (_second > _first)
    {
      const unsigned_common _diff_by_second{
          static_cast<unsigned_common>(_second - _first)};

      if (internal::fits<t_result>(_diff_by_second))
      {
        return static_cast<t_result>(_diff_by_second);
      }
    }
    else
    {
      using signed_common = std::make_signed_t<unsigned_common>;

      const unsigned_common _diff_by_first{
          static_cast<unsigned_common>(_first - _second)};

      if (internal::fits<signed_common>(_diff_by_first))
      {
        // please, look at tnct::math::test::test_028 in file
        // tnct/math/tst/sum.h
        const signed_common _signed_second{static_cast<signed_common>(
            -static_cast<signed_common>(_diff_by_first))};

        if (internal::fits<t_result>(_signed_second))
        {
          return static_cast<t_result>(_signed_second);
        }
      }
    }
  }
  return std::nullopt;
}

template <std::signed_integral t_result, std::unsigned_integral t_first,
          std::signed_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sum_different_operands_signed_result(t_first p_first, t_second p_second)
{
  return sum_different_operands_signed_result<t_result>(p_second, p_first);
}

template <std::unsigned_integral t_result, std::signed_integral t_first,
          std::unsigned_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sum_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  using unsigned_common =
      std::common_type_t<std::make_unsigned_t<t_first>, t_second>;

  if (p_first >= 0)
  {
    const unsigned_common _first{static_cast<unsigned_common>(p_first)};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};
    const auto            _maybe{
        internal::sum_same_operands<unsigned_common>(_first, _second)};
    if (_maybe && internal::fits<t_result>(*_maybe))
    {
      return static_cast<t_result>(*_maybe);
    }
  }
  else
  {
    const unsigned_common _first{static_cast<unsigned_common>(revert(p_first))};
    const unsigned_common _second{static_cast<unsigned_common>(p_second)};

    if (_second == _first)
    {
      return static_cast<t_result>(0);
    }
    else if (_second > _first)
    {
      const unsigned_common _diff_by_second{_second - _first};

      if (internal::fits<t_result>(_diff_by_second))
      {
        return static_cast<t_result>(_diff_by_second);
      }
    }
  }
  return std::nullopt;
}

template <std::unsigned_integral t_result, std::unsigned_integral t_first,
          std::signed_integral t_second>
[[nodiscard]] constexpr std::optional<t_result>
sum_different_operands_unsigned_result(t_first p_first, t_second p_second)
{
  return sum_different_operands_unsigned_result<t_result>(p_second, p_first);
}

} // namespace tnct::math::bus::internal

#endif
