#ifndef TNCT_MATH_BUS_MOVING_AVERAGE_H
#define TNCT_MATH_BUS_MOVING_AVERAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <optional>

#include "tnct/math/bus/mul.h"
#include "tnct/math/bus/sub.h"
#include "tnct/math/bus/sum.h"
#include "tnct/math/cpt/number.h"

namespace tnct::math::bus
{

///
/// The moving average is computed from the accumulated sum divided by size, and
/// the final value is converted to t_result.
/// If t_result is integral, the fractional part is discarded by conversion.
template <cpt::number t_result, cpt::number t_number, std::size_t t_amount>
struct moving_average
{
  static_assert(
      t_amount > 1,
      "at least two numbers are necessary to calculate a moving average");

  using result = t_result;
  using number = t_number;
  using index  = std::size_t;

  moving_average(number p_default = number{0});

  [[nodiscard]] std::optional<result> operator()(number p_number);

  static constexpr index size = t_amount;

private:
  using average = std::common_type_t<t_result, t_number>;

private:
  std::optional<index>     m_current{index{0}};
  std::array<number, size> m_values;
  std::optional<average>   m_last_sum{average{0}};
};

template <cpt::number t_result, cpt::number t_number, std::size_t t_amount>
moving_average<t_result, t_number, t_amount>::moving_average(number p_default)
    : m_last_sum{mul<average>(size, p_default)}
{
  m_values.fill(p_default);
}

template <cpt::number t_result, cpt::number t_number, std::size_t t_amount>
std::optional<t_result>
moving_average<t_result, t_number, t_amount>::operator()(number p_number)
{
  if (!m_last_sum || !m_current)
  {
    return std::nullopt;
  }

  if (*m_current >= size)
  {
    *m_current = 0;
  }

  m_last_sum = sub<average>(*m_last_sum, m_values[*m_current]);

  if (!m_last_sum)
  {
    return std::nullopt;
  }

  m_values[*m_current] = p_number;

  m_current = sum<index>(*m_current, index{1});

  if (!m_current)
  {
    return std::nullopt;
  }

  m_last_sum = sum<average>(*m_last_sum, p_number);

  if (!m_last_sum)
  {
    return std::nullopt;
  }

  return static_cast<result>(*m_last_sum / static_cast<double>(size));
}

} // namespace tnct::math::bus

#endif
