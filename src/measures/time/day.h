#ifndef TENACITAS_MEASURES_DAY_H
#define TENACITAS_MEASURES_DAY_H

#include <cstdint>
#include <iostream>

#include <measures/internal/amount_t.h>

namespace tenacitas {
namespace measures {
namespace time {

///
/// \brief The day struct represents a valid day value
///
struct day {

  /// \brief constructor creates a valid \p day if possible
  ///
  /// \tparam type of number used to create a \p day object
  ///
  /// \param p_value value used to create a \p day object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p day object
  template <typename t_int> explicit day(t_int p_value) { assign(p_value); }

  /// \brief day constructor from a \p amount_t<day>, i.e., \p days
  ///
  /// \param p_amount amount of days used to create a \p day
  explicit inline day(amount_t<day> p_amount) {
    auto _value = p_amount.value();
    assign(_value);
  }

  day(const day &) = default;
  day(day &&) = default;
  ~day() = default;

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_day
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const day &p_day) {
    p_out << static_cast<uint16_t>(p_day.m_value);
    return p_out;
  }

  /// \brief valid day values

  static const day _01;
  static const day _02;
  static const day _03;
  static const day _04;
  static const day _05;
  static const day _06;
  static const day _07;
  static const day _08;
  static const day _09;
  static const day _10;
  static const day _11;
  static const day _12;
  static const day _13;
  static const day _14;
  static const day _15;
  static const day _16;
  static const day _17;
  static const day _18;
  static const day _19;
  static const day _20;
  static const day _21;
  static const day _22;
  static const day _23;
  static const day _24;
  static const day _25;
  static const day _26;
  static const day _27;
  static const day _28;
  static const day _29;
  static const day _30;
  static const day _31;

  day &operator=(const day &) = default;
  day &operator=(day &&) = default;

  /// \brief day assignment from a \p amount_t<day>, i.e., \p days
  ///
  /// \param p_amount amount of days used to create a \p day
  inline day &operator=(amount_t<day> p_amount) {
    assign(p_amount.value());
    return *this;
  }

  /// \brief operator >
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is greater than \p p_day
  inline bool operator>(const day &p_day) const {
    return m_value > p_day.m_value;
  }

  /// \brief operator <
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is smaller than \p p_day
  inline bool operator<(const day &p_day) const {
    return m_value < p_day.m_value;
  }

  /// \brief operator >=
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is greater than, or equal to, \p p_day
  inline bool operator>=(const day &p_day) const {
    return m_value >= p_day.m_value;
  }

  /// \brief operator <=
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is smaller than, or equal to, \p p_day
  inline bool operator<=(const day &p_day) const {
    return m_value <= p_day.m_value;
  }

  /// \brief operator -=
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is equal to \p p_day
  inline bool operator==(const day &p_day) const {
    return m_value == p_day.m_value;
  }

  /// \brief operator -=
  ///
  /// \param p_day day to be compared
  ///
  /// \return \p true if \p this day is not equal to \p p_day
  inline bool operator!=(const day &p_day) const {
    return m_value != p_day.m_value;
  }

  template<typename t_type>
  t_type value() const { return static_cast<t_type>(m_value); }



private:
  template <typename t_int> void assign(t_int p_value) {
    if ((static_cast<decltype(m_value)>(p_value) > 31) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid day value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

private:
  ///
  /// \brief m_value
  ///
  uint8_t m_value;
};

///
/// \brief operator <invalid>
/// \param p_int
/// \return
///
inline day operator"" _day(unsigned long long p_int) { return day(p_int); }

} // namespace time
} // namespace measures
} // namespace tenacitas

#endif // DAY_H