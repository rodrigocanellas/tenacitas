#ifndef TENACITAS_CALENDAR_GREGORIAN_MINUTE_H
#define TENACITAS_CALENDAR_GREGORIAN_MINUTE_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace gregorian {
///
/// \brief The minute struct represents a valid minute value
///
struct minute {

  template <typename t_int> constexpr explicit minute(t_int p_value) {
    if ((p_value > 59) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid second value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const minute &p_inute) {
    p_out << static_cast<uint32_t>(p_inute.m_value);
    return p_out;
  }

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

  constexpr inline bool operator>(const minute &p_inute) const {
    return m_value > p_inute.m_value;
  }

  constexpr inline bool operator<(const minute &p_inute) const {
    return m_value < p_inute.m_value;
  }

  constexpr inline bool operator>=(const minute &p_inute) const {
    return m_value >= p_inute.m_value;
  }

  constexpr inline bool operator<=(const minute &p_inute) const {
    return m_value <= p_inute.m_value;
  }

  constexpr inline bool operator==(const minute &p_inute) const {
    return m_value == p_inute.m_value;
  }

  constexpr inline bool operator!=(const minute &p_inute) const {
    return m_value != p_inute.m_value;
  }

  //  static inline seconds get_seconds() { return seconds(60); }

  // private:
  //  constexpr inline uint8_t value() const { return m_value; }

private:
  uint8_t m_value;
};

inline minute operator"" _min(unsigned long long p_int) {
  switch (p_int) {
  case 0:
    return minute::_00;
  case 1:
    return minute::_01;
  case 2:
    return minute::_02;
  case 3:
    return minute::_03;
  case 4:
    return minute::_04;
  case 5:
    return minute::_05;
  case 6:
    return minute::_06;
  case 7:
    return minute::_07;
  case 8:
    return minute::_08;
  case 9:
    return minute::_09;
  case 10:
    return minute::_10;
  case 11:
    return minute::_11;
  case 12:
    return minute::_12;
  case 13:
    return minute::_13;
  case 14:
    return minute::_14;
  case 15:
    return minute::_15;
  case 16:
    return minute::_16;
  case 17:
    return minute::_17;
  case 18:
    return minute::_18;
  case 19:
    return minute::_19;
  case 20:
    return minute::_20;
  case 21:
    return minute::_21;
  case 22:
    return minute::_22;
  case 23:
    return minute::_23;
  case 24:
    return minute::_24;
  case 25:
    return minute::_25;
  case 26:
    return minute::_26;
  case 27:
    return minute::_27;
  case 28:
    return minute::_28;
  case 29:
    return minute::_29;
  case 30:
    return minute::_30;
  case 31:
    return minute::_31;
  case 32:
    return minute::_32;
  case 33:
    return minute::_33;
  case 34:
    return minute::_34;
  case 35:
    return minute::_35;
  case 36:
    return minute::_36;
  case 37:
    return minute::_37;
  case 38:
    return minute::_38;
  case 39:
    return minute::_39;
  case 40:
    return minute::_40;
  case 41:
    return minute::_41;
  case 42:
    return minute::_42;
  case 43:
    return minute::_43;
  case 44:
    return minute::_44;
  case 45:
    return minute::_45;
  case 46:
    return minute::_46;
  case 47:
    return minute::_47;
  case 48:
    return minute::_48;
  case 49:
    return minute::_49;
  case 50:
    return minute::_50;
  case 51:
    return minute::_51;
  case 52:
    return minute::_52;
  case 53:
    return minute::_53;
  case 54:
    return minute::_54;
  case 55:
    return minute::_55;
  case 56:
    return minute::_56;
  case 57:
    return minute::_57;
  case 58:
    return minute::_58;
  case 59:
    return minute::_59;
  default:
    throw std::runtime_error(std::to_string(p_int) +
                             " is not a valid minute value");
  }
}
} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MINUTE_H
