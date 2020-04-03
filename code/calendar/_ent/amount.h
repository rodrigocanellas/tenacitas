#ifndef TENACITAS_CALENDAR_ENT_AMOUNT_H
#define TENACITAS_CALENDAR_ENT_AMOUNT_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace _ent {

/// amount represents an amount of some type of time
///
/// @tparam t_time_precision defines a type of time, like 'day', 'month'
template<typename t_time_precision>
struct amount_t
{
  template<typename t_int>
  inline explicit amount_t(t_int p_int)
    : m_value(static_cast<decltype(m_value)>(p_int))
  {}

  inline amount_t(t_time_precision p_time)
    : m_value(static_cast<decltype(m_value)>(p_time.value()))
  {}

  inline amount_t()
    : m_value(0)
  {}

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const amount_t& p_amount)
  {
    p_out << p_amount.m_value;
    return p_out;
  }

  inline amount_t& operator++()
  {
    ++m_value;
    return *this;
  }

  inline amount_t& operator--()
  {
    --m_value;
    return *this;
  }

  inline amount_t& operator++(int)
  {
    m_value++;
    return *this;
  }

  inline amount_t& operator--(int)
  {
    m_value--;
    return *this;
  }

  inline amount_t operator+(amount_t p_amount_t)
  {
    return amount_t(m_value + p_amount_t.m_value);
  }

  inline amount_t operator-(amount_t p_amount_t)
  {
    return amount_t(m_value - p_amount_t.m_value);
  }

  inline amount_t& operator+=(amount_t p_amount_t)
  {
    m_value += p_amount_t.m_value;
    return *this;
  }

  inline amount_t& operator-=(amount_t p_amount_t)
  {
    m_value -= p_amount_t.m_value;
    return *this;
  }

  inline amount_t operator*(amount_t p_amount_t) const
  {
    return amount_t(m_value * p_amount_t.m_value);
  }

  inline amount_t& operator*=(amount_t p_amount_t)
  {
    m_value *= p_amount_t.m_value;
    return *this;
  }

  inline amount_t operator/(amount_t p_amount_t) const
  {
    return amount_t(m_value / p_amount_t.m_value);
  }

  inline amount_t& operator/=(amount_t p_amount_t)
  {
    m_value /= p_amount_t.m_value;
    return *this;
  }

  inline amount_t operator%(amount_t p_amount_t) const
  {
    return amount_t(m_value % p_amount_t.m_value);
  }

  inline bool operator>(const amount_t& p_amount_t) const
  {
    return (m_value > p_amount_t.template get<decltype(m_value)>());
  }

  inline bool operator<(const amount_t& p_amount_t) const
  {
    return (m_value < p_amount_t.template get<decltype(m_value)>());
  }

  inline bool operator>=(const amount_t& p_amount_t) const
  {
    return (m_value >= p_amount_t.template get<decltype(m_value)>());
  }

  inline bool operator<=(const amount_t& p_amount_t) const
  {
    return (m_value <= p_amount_t.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline amount_t operator+(t_amount p_amount)
  {
    return amount(m_value + p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline amount_t operator-(t_amount p_amount)
  {
    return amount(m_value - p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline amount_t& operator+=(t_amount p_amount)
  {
    m_value += p_amount.template get<decltype(m_value)>();
    return *this;
  }

  template<typename t_amount>
  inline amount_t& operator-=(t_amount p_amount)
  {
    m_value -= p_amount.template get<decltype(m_value)>();
    return *this;
  }

  template<typename t_amount>
  inline amount_t operator*(t_amount p_amount) const
  {
    return amount_t(m_value * p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline amount_t& operator*=(t_amount p_amount)
  {
    m_value *= p_amount.template get<decltype(m_value)>();
    return *this;
  }

  template<typename t_amount>
  inline amount_t operator/(t_amount p_amount) const
  {
    return amount_t(m_value / p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline amount_t& operator/=(t_amount p_amount)
  {
    m_value /= p_amount.template get<decltype(m_value)>();
    return *this;
  }

  template<typename t_amount>
  inline amount_t operator%(t_amount p_amount) const
  {
    return amount_t(m_value % p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline bool operator>(const t_amount& p_amount) const
  {
    return (m_value > p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline bool operator<(const t_amount& p_amount) const
  {
    return (m_value < p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline bool operator>=(const t_amount& p_amount) const
  {
    return (m_value >= p_amount.template get<decltype(m_value)>());
  }

  template<typename t_amount>
  inline bool operator<=(const t_amount& p_amount) const
  {
    return (m_value <= p_amount.template get<decltype(m_value)>());
  }

  /// explicit cast to some type of number
  ///
  /// @tparam t_int is a type of number
  template<typename t_number>
  inline t_number get() const
  {
    return static_cast<t_number>(m_value);
  }

private:
  int64_t m_value;
};

} // namespace _ent
} // namespace calendar
} // namespace tenacitas

#endif // AMOUNT_H
