#ifndef TENACITAS_CALENDAR_GREGORIAN_HOUR_H
#define TENACITAS_CALENDAR_GREGORIAN_HOUR_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/amount.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The hour struct represents a valid hour value
///
struct hour {

  /// \brief constructor creates a valid \p hour object if possible
  ///
  /// \tparam type of number used to create a \p hour object
  ///
  /// \param p_value value used to create a \p hour object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p hour object
  ///
  template <typename t_int> constexpr explicit hour(t_int p_value) {
    assign(p_value);
  }

  ///
  /// \brief hour constructor from a \p amount_t<hour>, i.e., \p hours
  /// \param p_amount amount of hours used to create a \p hour
  ///
  constexpr explicit inline hour(amount_t<hour> p_amount)
      : hour(p_amount.value()) {}

  ///
  /// \brief operator << output stream
  /// \param p_out type of stream
  /// \param p_hour \p hour object
  /// \return \p p_out object
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const hour &p_hour) {
    p_out << static_cast<uint16_t>(p_hour.m_value);
    return p_out;
  }

  /// \brief valid hour values

  static const hour _00;
  static const hour _01;
  static const hour _02;
  static const hour _03;
  static const hour _04;
  static const hour _05;
  static const hour _06;
  static const hour _07;
  static const hour _08;
  static const hour _09;
  static const hour _10;
  static const hour _11;
  static const hour _12;
  static const hour _13;
  static const hour _14;
  static const hour _15;
  static const hour _16;
  static const hour _17;
  static const hour _18;
  static const hour _19;
  static const hour _20;
  static const hour _21;
  static const hour _22;
  static const hour _23;

  ///
  /// \brief hour assignment from a \p amount_t<hour>, i.e., \p hours
  /// \param p_amount amount of hours used to create a \p hour
  ///
  constexpr inline hour &operator=(amount_t<hour> p_amount) {
    assign(p_amount.value());
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator>(const hour &p_hour) const {
    return m_value > p_hour.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator<(const hour &p_hour) const {
    return m_value < p_hour.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator>=(const hour &p_hour) const {
    return m_value >= p_hour.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator<=(const hour &p_hour) const {
    return m_value <= p_hour.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator==(const hour &p_hour) const {
    return m_value == p_hour.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_hour
  /// \return
  ///
  constexpr inline bool operator!=(const hour &p_hour) const {
    return m_value != p_hour.m_value;
  }

  constexpr auto value() const { return m_value; }

private:
  template <typename t_int> constexpr void assign(t_int p_value) {
    if ((p_value > 23) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid second value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

private:
  ///
  /// \brief m_value
  ///
  uint16_t m_value;
};

///
/// \brief operator <invalid>
/// \param p_int
/// \return
///
inline constexpr hour operator"" _hr(unsigned long long p_int) {
  return hour(p_int);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // HOUR_H
