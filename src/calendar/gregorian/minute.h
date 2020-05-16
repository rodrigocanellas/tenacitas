#ifndef TENACITAS_CALENDAR_GREGORIAN_MINUTE_H
#define TENACITAS_CALENDAR_GREGORIAN_MINUTE_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/internal/amount.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {
///
/// \brief The minute struct represents a valid minute value
///
struct minute {

  /// \brief constructor creates a valid \p minute if possible
  ///
  /// \tparam type of number used to create a \p minute object
  ///
  /// \param p_value value used to create a \p minute object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p minute object
  ///
  template <typename t_int> constexpr explicit minute(t_int p_value) {
    assign(p_value);
  }

  ///
  /// \brief minute constructor from a \p amount_t<minute>, i.e., \p minutes
  /// \param p_amount amount of minutes used to create a \p minute
  ///
  constexpr explicit inline minute(amount_t<minute> p_amount)
      : minute(p_amount.value()) {}

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_minute
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const minute &p_minute) {
    p_out << static_cast<uint32_t>(p_minute.m_value);
    return p_out;
  }

  /// \brief valid minute values

  static const minute _00;
  static const minute _01;
  static const minute _02;
  static const minute _03;
  static const minute _04;
  static const minute _05;
  static const minute _06;
  static const minute _07;
  static const minute _08;
  static const minute _09;
  static const minute _10;
  static const minute _11;
  static const minute _12;
  static const minute _13;
  static const minute _14;
  static const minute _15;
  static const minute _16;
  static const minute _17;
  static const minute _18;
  static const minute _19;
  static const minute _20;
  static const minute _21;
  static const minute _22;
  static const minute _23;
  static const minute _24;
  static const minute _25;
  static const minute _26;
  static const minute _27;
  static const minute _28;
  static const minute _29;
  static const minute _30;
  static const minute _31;
  static const minute _32;
  static const minute _33;
  static const minute _34;
  static const minute _35;
  static const minute _36;
  static const minute _37;
  static const minute _38;
  static const minute _39;
  static const minute _40;
  static const minute _41;
  static const minute _42;
  static const minute _43;
  static const minute _44;
  static const minute _45;
  static const minute _46;
  static const minute _47;
  static const minute _48;
  static const minute _49;
  static const minute _50;
  static const minute _51;
  static const minute _52;
  static const minute _53;
  static const minute _54;
  static const minute _55;
  static const minute _56;
  static const minute _57;
  static const minute _58;
  static const minute _59;

  ///
  /// \brief minute assignment from a \p amount_t<minute>, i.e., \p minutes
  /// \param p_amount amount of minutes used to create a \p minute
  ///
  constexpr inline minute &operator=(amount_t<minute> p_amount) {
    assign(p_amount.value());
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator>(const minute &p_minute) const {
    return m_value > p_minute.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator<(const minute &p_minute) const {
    return m_value < p_minute.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator>=(const minute &p_minute) const {
    return m_value >= p_minute.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator<=(const minute &p_minute) const {
    return m_value <= p_minute.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator==(const minute &p_minute) const {
    return m_value == p_minute.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_minute
  /// \return
  ///
  constexpr inline bool operator!=(const minute &p_minute) const {
    return m_value != p_minute.m_value;
  }

  constexpr auto value() const { return m_value; }

private:
  template <typename t_int> constexpr void assign(t_int p_value) {
    if ((static_cast<decltype(m_value)>(p_value) > 59) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid day value");
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
inline constexpr minute operator"" _min(unsigned long long p_int) {
  return minute(p_int);
}
} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MINUTE_H
