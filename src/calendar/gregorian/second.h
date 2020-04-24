#ifndef TENACITAS_CALENDAR_GREGORIAN_SECOND_H
#define TENACITAS_CALENDAR_GREGORIAN_SECOND_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace gregorian {
///
/// \brief The second struct represents a valid second value
///
struct second {

  /// \brief constructor creates a valid \p hour second if possible
  ///
  /// \tparam type of number used to create a \p second object
  ///
  /// \param p_value value used to create a \p second object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p second object
  ///
  template <typename t_int> constexpr explicit second(t_int p_value) {
    if ((p_value > 59) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid second value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_second
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const second &p_second) {
    p_out << static_cast<uint16_t>(p_second.m_value);
    return p_out;
  }

  /// \brief valid second values

  static const second _00;
  static const second _01;
  static const second _02;
  static const second _03;
  static const second _04;
  static const second _05;
  static const second _06;
  static const second _07;
  static const second _08;
  static const second _09;
  static const second _10;
  static const second _11;
  static const second _12;
  static const second _13;
  static const second _14;
  static const second _15;
  static const second _16;
  static const second _17;
  static const second _18;
  static const second _19;
  static const second _20;
  static const second _21;
  static const second _22;
  static const second _23;
  static const second _24;
  static const second _25;
  static const second _26;
  static const second _27;
  static const second _28;
  static const second _29;
  static const second _30;
  static const second _31;
  static const second _32;
  static const second _33;
  static const second _34;
  static const second _35;
  static const second _36;
  static const second _37;
  static const second _38;
  static const second _39;
  static const second _40;
  static const second _41;
  static const second _42;
  static const second _43;
  static const second _44;
  static const second _45;
  static const second _46;
  static const second _47;
  static const second _48;
  static const second _49;
  static const second _50;
  static const second _51;
  static const second _52;
  static const second _53;
  static const second _54;
  static const second _55;
  static const second _56;
  static const second _57;
  static const second _58;
  static const second _59;

  ///
  /// \brief operator >
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator>(const second &p_second) const {
    return m_value > p_second.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator<(const second &p_second) const {
    return m_value < p_second.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator>=(const second &p_second) const {
    return m_value >= p_second.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator<=(const second &p_second) const {
    return m_value <= p_second.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator==(const second &p_second) const {
    return m_value == p_second.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_second
  /// \return
  ///
  constexpr inline bool operator!=(const second &p_second) const {
    return m_value != p_second.m_value;
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
inline constexpr second operator"" _sec(unsigned long long p_int) {
  return second(p_int);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // SECOND_H
