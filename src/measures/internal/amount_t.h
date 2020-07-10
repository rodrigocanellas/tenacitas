#ifndef TENACITAS_MEASURES_AMOUNT_H
#define TENACITAS_MEASURES_AMOUNT_H

#include <cmath>
#include <cstdint>
#include <iostream>

#include <measures/internal/convert_t.h>

namespace tenacitas {
namespace measures {

///
/// \brief Amount of a certain measurement unit, line <tt>amount_t<days></tt>
///
/// \tparam t_unit is the  measurement unit
///
template <typename t_unit, typename t_value = double> struct amount_t {
  inline amount_t(int8_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const int8_t &p_value) : m_value(p_value) {}
  inline amount_t(uint8_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const uint8_t &p_value) : m_value(p_value) {}
  inline amount_t(int16_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const int16_t &p_value) : m_value(p_value) {}
  inline amount_t(uint16_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const uint16_t &p_value) : m_value(p_value) {}
  inline amount_t(int32_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const int32_t &p_value) : m_value(p_value) {}
  inline amount_t(uint32_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const uint32_t &p_value) : m_value(p_value) {}
  inline amount_t(int64_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const int64_t &p_value) : m_value(p_value) {}
  inline amount_t(uint64_t &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const uint64_t &p_value) : m_value(p_value) {}
  inline amount_t(float &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const float &p_value) : m_value(p_value) {}
  inline amount_t(double &&p_value) : m_value(std::move(p_value)) {}
  inline amount_t(const double &p_value) : m_value(p_value) {}
  amount_t(const amount_t &) = default;
  amount_t(amount_t &&) = default;
  amount_t &operator=(const amount_t &) = default;
  amount_t &operator=(amount_t &&) = default;

  /// \brief constructor that performs conversion to any other type, as far as
  /// there is a 'convert' function specialization to execute the conversion
  ///
  /// \tparam t_other_amount other type of amount to be converted
  ///
  /// \param p_from amount of the \p t_other_amount to be converted
  template <typename t_other_amount>
  inline amount_t(t_other_amount p_amount)
      : amount_t(convert<t_other_amount, amount_t>(p_amount)) {}

  inline amount_t(t_unit p_unit) : m_value(p_unit.value()) {}

  friend std::ostream &operator<<(std::ostream &p_out, const amount_t &p_mm) {
    p_out << p_mm.m_value;
    return p_out;
  }

  inline amount_t operator+(const amount_t &p_amount) const {
    return amount_t(m_value + p_amount.value());
  }

  inline amount_t &operator+=(const amount_t &p_amount) {
    m_value += p_amount.value();
    return *this;
  }

  inline amount_t operator-(const amount_t &p_amount) const {
    return amount_t(m_value - p_amount.value());
  }

  inline amount_t &operator-=(const amount_t &p_amount) {
    m_value -= p_amount.value();
    return *this;
  }

  inline void operator++() { ++m_value; }

  inline void operator++(int) { m_value++; }

  inline void operator--() { --m_value; }

  inline void operator--(int) { m_value--; }

  inline amount_t operator*(double p_value) const { return m_value * p_value; }

  inline amount_t operator*=(double p_value) {
    m_value *= p_value;
    return *this;
  }

  inline amount_t operator/(double p_value) const { return m_value / p_value; }

  inline amount_t operator/=(double p_value) {
    m_value /= p_value;
    return *this;
  }

  inline int64_t operator%(double p_value) const {
    //    return std::remainder(m_value, p_value);
    return static_cast<int64_t>(m_value) % static_cast<int64_t>(p_value);
  }

  inline bool operator==(const amount_t &p_amount) const {
    return m_value == p_amount.m_value;
  }

  inline bool operator!=(const amount_t &p_amount) const {
    return m_value != p_amount.m_value;
  }

  inline bool operator<(const amount_t &p_amount) const {
    return m_value < p_amount.m_value;
  }

  inline bool operator<=(const amount_t &p_amount) const {
    return m_value <= p_amount.m_value;
  }

  inline bool operator>(const amount_t &p_amount) const {
    return m_value > p_amount.m_value;
  }

  inline bool operator>=(const amount_t &p_amount) const {
    return m_value >= p_amount.m_value;
  }

  /// \return the raw value of the amount
  inline double value() const { return m_value; }

  inline int64_t integer() const { return static_cast<int64_t>(m_value); }

  inline double decimal() const {
    return m_value - static_cast<int64_t>(m_value);
  }

private:
  /// \brief the raw value of the amount
  t_value m_value = {0};
};

} // namespace measures
} // namespace tenacitas

#endif // AMOUNT_H
