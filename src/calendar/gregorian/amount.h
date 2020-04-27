#ifndef TENACITAS_CALENDAR_GREGORIAN_AMOUNT_H
#define TENACITAS_CALENDAR_GREGORIAN_AMOUNT_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/convert.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

/// \brief amount represents an amount of some type of time
///
/// \tparam int64_t is the type of value of the amount
///
/// \tparam t_time defines a type of time, like 'day', 'month'
///
template <typename t_time> struct amount_t {

  typedef t_time time;

  constexpr inline explicit amount_t(double p_value = 0) : m_value(p_value) {}

  constexpr amount_t(const amount_t &) = default;

  constexpr amount_t(amount_t &&) = default;

  constexpr amount_t &operator=(const amount_t &) = default;

  constexpr amount_t &operator=(amount_t &&) = default;

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const amount_t &p_amount) {
    p_out << p_amount.m_value;
    return p_out;
  }

  constexpr inline operator double() const { return m_value; }

  constexpr inline amount_t &operator++() {
    ++m_value;
    return *this;
  }

  constexpr inline amount_t &operator--() {
    --m_value;
    return *this;
  }

  constexpr inline amount_t &operator++(int) {
    m_value++;
    return *this;
  }

  constexpr inline amount_t &operator--(int) {
    m_value--;
    return *this;
  }

  constexpr inline bool operator==(const amount_t &p_amount) const {
    return m_value == p_amount.m_value;
  }

  constexpr inline bool operator!=(const amount_t &p_amount) const {
    return m_value != p_amount.m_value;
  }

  constexpr inline amount_t operator+(const amount_t &p_amount) const {
    return amount_t(m_value + p_amount.m_value);
  }

  constexpr inline amount_t operator-(const amount_t &p_amount) const {
    return amount_t(m_value - p_amount.m_value);
  }

  template <typename t_other_amount>
  constexpr inline amount_t
  operator+(const t_other_amount &p_other_amount) const {
    amount_t _amount = convert<t_other_amount, amount_t>(p_other_amount);
    return (*this) + _amount;
  }

  template <typename t_other_amount>
  constexpr inline amount_t
  operator-(const t_other_amount &p_other_amount) const {
    amount_t _amount = convert<t_other_amount, amount_t>(p_other_amount);
    return (*this) - _amount;
  }

  constexpr inline amount_t &operator+=(const amount_t &p_amount) {
    m_value += p_amount.m_value;
    return *this;
  }

  constexpr inline amount_t &operator-=(const amount_t &p_amount) {
    m_value -= p_amount.m_value;
    return *this;
  }

  constexpr inline amount_t operator*(const amount_t &p_amount) const {
    return amount_t(m_value * p_amount.m_value);
  }

  constexpr inline amount_t &operator*=(const amount_t &p_amount) {
    m_value *= p_amount.m_value;
    return *this;
  }

  constexpr inline amount_t operator/(const amount_t &p_amount) const {
    return amount_t(m_value / p_amount.m_value);
  }

  constexpr inline amount_t &operator/=(const amount_t &p_amount) {
    m_value /= p_amount.m_value;
    return *this;
  }

  constexpr inline amount_t operator%(const amount_t &p_amount) const {
    return amount_t(static_cast<int64_t>(m_value) %
                    static_cast<int64_t>(p_amount.m_value));
  }

  constexpr inline bool operator>(const amount_t &p_amount) const {
    return (m_value > p_amount.m_value);
  }

  constexpr inline bool operator<(const amount_t &p_amount) const {
    return (m_value < p_amount.m_value);
  }

  constexpr inline bool operator>=(const amount_t &p_amount) const {
    return (m_value >= p_amount.m_value);
  }

  constexpr inline bool operator<=(const amount_t &p_amount) const {
    return (m_value <= p_amount.m_value);
  }

private:
  double m_value;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // AMOUNT_H
