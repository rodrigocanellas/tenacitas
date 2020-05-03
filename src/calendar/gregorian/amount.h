#ifndef TENACITAS_CALENDAR_GREGORIAN_AMOUNT_H
#define TENACITAS_CALENDAR_GREGORIAN_AMOUNT_H

#include <cmath>
#include <cstdint>
#include <iostream>

#include <calendar/gregorian/convert.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief Amount of a certain measurement unit, line <tt>amount_t<days></tt>
///
/// \tparam t_unit is the  measurement unit
///
template <typename t_unit> struct amount_t {
  inline constexpr amount_t(int8_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const int8_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(uint8_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const uint8_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(int16_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const int16_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(uint16_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const uint16_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(int32_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const int32_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(uint32_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const uint32_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(int64_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const int64_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(uint64_t &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const uint64_t &p_value) : m_value(p_value) {}
  inline constexpr amount_t(float &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const float &p_value) : m_value(p_value) {}
  inline constexpr amount_t(double &&p_value) : m_value(std::move(p_value)) {}
  inline constexpr amount_t(const double &p_value) : m_value(p_value) {}
  constexpr amount_t(const amount_t &) = default;
  constexpr amount_t(amount_t &&) = default;
  amount_t &operator=(const amount_t &) = default;
  amount_t &operator=(amount_t &&) = default;

  /// \brief constructor that performs conversion to any other type, as far as
  /// there is a 'convert' function specialization to execute the conversion
  ///
  /// \tparam t_other_amount other type of amount to be converted
  ///
  /// \param p_from amount of the \p t_other_amount to be converted
  template <typename t_other_amount>
  inline constexpr amount_t(t_other_amount p_amount)
      : amount_t(convert<t_other_amount, amount_t>(p_amount)) {}

  inline constexpr amount_t(t_unit p_unit) : m_value(p_unit.value()) {}

  friend std::ostream &operator<<(std::ostream &p_out, const amount_t &p_mm) {
    p_out << p_mm.m_value;
    return p_out;
  }

  inline constexpr amount_t operator+(const amount_t &p_amount) const {
    return amount_t(m_value + p_amount.value());
  }

  inline amount_t &operator+=(const amount_t &p_amount) {
    m_value += p_amount.value();
    return *this;
  }

  inline constexpr amount_t operator-(const amount_t &p_amount) const {
    return amount_t(m_value - p_amount.value());
  }

  inline amount_t &operator-=(const amount_t &p_amount) const {
    m_value -= p_amount.value();
    return *this;
  }

  inline void operator++() { ++m_value; }

  inline void operator++(int) { m_value++; }

  inline void operator--() { --m_value; }

  inline void operator--(int) { m_value--; }

  inline constexpr amount_t operator*(double p_value) const {
    return m_value * p_value;
  }

  inline amount_t operator*=(double p_value) {
    m_value *= p_value;
    return *this;
  }

  inline constexpr amount_t operator/(double p_value) const {
    return m_value / p_value;
  }

  inline amount_t operator/=(double p_value) {
    m_value /= p_value;
    return *this;
  }

  inline constexpr double operator%(double p_value) const {
    return std::remainder(m_value, p_value);
  }

  inline constexpr bool operator==(const amount_t &p_amount) const {
    return m_value == p_amount.m_value;
  }

  inline constexpr bool operator!=(const amount_t &p_amount) const {
    return m_value != p_amount.m_value;
  }

  inline constexpr bool operator<(const amount_t &p_amount) const {
    return m_value < p_amount.m_value;
  }

  inline constexpr bool operator<=(const amount_t &p_amount) const {
    return m_value <= p_amount.m_value;
  }

  inline constexpr bool operator>(const amount_t &p_amount) const {
    return m_value > p_amount.m_value;
  }

  inline constexpr bool operator>=(const amount_t &p_amount) const {
    return m_value >= p_amount.m_value;
  }

  /// \return the raw value of the amount
  inline constexpr double value() const { return m_value; }

private:
  /// \brief the raw value of the amount
  double m_value = {0.0};
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // AMOUNT_H
