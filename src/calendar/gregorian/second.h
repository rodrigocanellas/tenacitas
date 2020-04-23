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

  template <typename t_int> constexpr explicit second(t_int p_value) {
    if ((p_value > 59) || (p_value < 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid second value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const second &p_second) {
    p_out << static_cast<uint16_t>(p_second.m_value);
    return p_out;
  }

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

  constexpr inline bool operator>(const second &p_second) const {
    return m_value > p_second.m_value;
  }

  constexpr inline bool operator<(const second &p_second) const {
    return m_value < p_second.m_value;
  }

  constexpr inline bool operator>=(const second &p_second) const {
    return m_value >= p_second.m_value;
  }

  constexpr inline bool operator<=(const second &p_second) const {
    return m_value <= p_second.m_value;
  }

  constexpr inline bool operator==(const second &p_second) const {
    return m_value == p_second.m_value;
  }

  constexpr inline bool operator!=(const second &p_second) const {
    return m_value != p_second.m_value;
  }

  // private:
  //  constexpr inline uint8_t value() const { return m_value; }

private:
  uint8_t m_value;
};

inline second operator""_sec(unsigned long long p_int) {
  if (p_int > 59) {
    throw std::runtime_error(std::to_string(p_int) +
                             " is not a valid second value");
  }

  switch (p_int) {
  case 0:
    return second::_00;
  case 1:
    return second::_01;
  case 2:
    return second::_02;
  case 3:
    return second::_03;
  case 4:
    return second::_04;
  case 5:
    return second::_05;
  case 6:
    return second::_06;
  case 7:
    return second::_07;
  case 8:
    return second::_08;
  case 9:
    return second::_09;
  case 10:
    return second::_10;
  case 11:
    return second::_11;
  case 12:
    return second::_12;
  case 13:
    return second::_13;
  case 14:
    return second::_14;
  case 15:
    return second::_15;
  case 16:
    return second::_16;
  case 17:
    return second::_17;
  case 18:
    return second::_18;
  case 19:
    return second::_19;
  case 20:
    return second::_20;
  case 21:
    return second::_21;
  case 22:
    return second::_22;
  case 23:
    return second::_23;
  case 24:
    return second::_24;
  case 25:
    return second::_25;
  case 26:
    return second::_26;
  case 27:
    return second::_27;
  case 28:
    return second::_28;
  case 29:
    return second::_29;
  case 30:
    return second::_30;
  case 31:
    return second::_31;
  case 32:
    return second::_32;
  case 33:
    return second::_33;
  case 34:
    return second::_34;
  case 35:
    return second::_35;
  case 36:
    return second::_36;
  case 37:
    return second::_37;
  case 38:
    return second::_38;
  case 39:
    return second::_39;
  case 40:
    return second::_40;
  case 41:
    return second::_41;
  case 42:
    return second::_42;
  case 43:
    return second::_43;
  case 44:
    return second::_44;
  case 45:
    return second::_45;
  case 46:
    return second::_46;
  case 47:
    return second::_47;
  case 48:
    return second::_48;
  case 49:
    return second::_49;
  case 50:
    return second::_50;
  case 51:
    return second::_51;
  case 52:
    return second::_52;
  case 53:
    return second::_53;
  case 54:
    return second::_54;
  case 55:
    return second::_55;
  case 56:
    return second::_56;
  case 57:
    return second::_57;
  case 58:
    return second::_58;
  default:
    return second::_59;
  }
}
} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // SECOND_H
