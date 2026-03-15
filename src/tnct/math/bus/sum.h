#ifndef TNCT_MATH_BUS_SUM_H
#define TNCT_MATH_BUS_SUM_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <optional>
#include <type_traits>

#include "tnct/math/bus/internal/sum.h"
#include "tnct/math/cpt/number.h"

namespace tnct::math::bus
{

template <std::integral t_result, std::integral t_first, std::integral t_second>
[[nodiscard]] constexpr std::optional<t_result> sum(t_first  p_first,
                                                    t_second p_second)
{
  if constexpr (std::is_same_v<t_first, t_second>)
  {
    return internal::sum_same_operands<t_result>(p_first, p_second);
  }
  else if constexpr (std::is_signed_v<t_result>)
  {
    return internal::sum_different_operands_signed_result<t_result>(p_first,
                                                                    p_second);
  }
  else
  {
    return internal::sum_different_operands_unsigned_result<t_result>(p_first,
                                                                      p_second);
  }
}

template <std::floating_point t_result, math::cpt::number t_first,
          math::cpt::number t_second>
[[nodiscard]] constexpr std::optional<t_result> sum(t_first  p_first,
                                                    t_second p_second)
{
  const t_result _first{static_cast<t_result>(p_first)};
  if (!std::isfinite(_first))
  {
    return std::nullopt;
  }

  const t_result _second{static_cast<t_result>(p_second)};
  if (!std::isfinite(_second))
  {
    return std::nullopt;
  }

  const t_result _result{_first + _second};
  if (!std::isfinite(_result))
  {
    return std::nullopt;
  }

  return _result;
}

} // namespace tnct::math::bus

#endif
