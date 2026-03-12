#ifndef TNCT_MATH_BUS_MOVING_AVERAGE_H
#define TNCT_MATH_BUS_MOVING_AVERAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <optional>

namespace tnct::math::bus
{

template <typename t_result, typename t_number, std::size_t t_amount>
requires((std::integral<t_result> || std::floating_point<t_result>)
         && (std::integral<t_number> || std::floating_point<t_number>))
struct moving_average
{
  static_assert(
      t_amount > 1,
      "at least two numbers are necessary to calculate a moving average");

  using result = t_result;
  using number = t_number;

  moving_average(number p_default = number{0})
      : m_default{p_default}, m_last_sum(amount * p_default)
  {
    m_values.fill(p_default);
  }

  [[nodiscard]] std::optional<result> operator()(number p_number)
  {
    if (m_current == amount)
    {
      m_current = 0;
    }
    m_last_sum -= m_values[m_current];

    m_values[m_current] = p_number;

    ++m_current;
    m_last_sum += p_number;

    return static_cast<result>(m_last_sum / static_cast<double>(amount));
  }

  static constexpr std::size_t amount = t_amount;

private:
  std::size_t                m_current{0};
  std::array<number, amount> m_values;
  number                     m_default;
  result                     m_last_sum{0};
};

} // namespace tnct::math::bus

#endif
