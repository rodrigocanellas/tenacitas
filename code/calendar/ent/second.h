#ifndef TENACITAS_CALENDAR_ENTITIES_SECOND_H
#define TENACITAS_CALENDAR_ENTITIES_SECOND_H

#include <cstdint>
#include <iostream>

#include <calendar/ent/amount.h>

namespace tenacitas {
namespace calendar {
namespace ent {

/**
 * @brief The second struct
 */
struct second
{
  friend struct amount<second>;

  second() = delete;

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const second& p_second)
  {
    p_out << static_cast<uint32_t>(p_second.m_value);
    return p_out;
  }

  static const second s00;
  static const second s01;
  static const second s02;
  static const second s03;
  static const second s04;
  static const second s05;
  static const second s06;
  static const second s07;
  static const second s08;
  static const second s09;
  static const second s10;
  static const second s11;
  static const second s12;
  static const second s13;
  static const second s14;
  static const second s15;
  static const second s16;
  static const second s17;
  static const second s18;
  static const second s19;
  static const second s20;
  static const second s21;
  static const second s22;
  static const second s23;
  static const second s24;
  static const second s25;
  static const second s26;
  static const second s27;
  static const second s28;
  static const second s29;
  static const second s30;
  static const second s31;
  static const second s32;
  static const second s33;
  static const second s34;
  static const second s35;
  static const second s36;
  static const second s37;
  static const second s38;
  static const second s39;
  static const second s40;
  static const second s41;
  static const second s42;
  static const second s43;
  static const second s44;
  static const second s45;
  static const second s46;
  static const second s47;
  static const second s48;
  static const second s49;
  static const second s50;
  static const second s51;
  static const second s52;
  static const second s53;
  static const second s54;
  static const second s55;
  static const second s56;
  static const second s57;
  static const second s58;
  static const second s59;

  template<typename t_int>
  static second create(t_int p_int)
  {
    switch (p_int) {
      case 0:
        return second::s00;
      case 1:
        return second::s01;
      case 2:
        return second::s02;
      case 3:
        return second::s03;
      case 4:
        return second::s04;
      case 5:
        return second::s05;
      case 6:
        return second::s06;
      case 7:
        return second::s07;
      case 8:
        return second::s08;
      case 9:
        return second::s09;
      case 10:
        return second::s10;
      case 11:
        return second::s11;
      case 12:
        return second::s12;
      case 13:
        return second::s13;
      case 14:
        return second::s14;
      case 15:
        return second::s15;
      case 16:
        return second::s16;
      case 17:
        return second::s17;
      case 18:
        return second::s18;
      case 19:
        return second::s19;
      case 20:
        return second::s20;
      case 21:
        return second::s21;
      case 22:
        return second::s22;
      case 23:
        return second::s23;
      case 24:
        return second::s24;
      case 25:
        return second::s25;
      case 26:
        return second::s26;
      case 27:
        return second::s27;
      case 28:
        return second::s28;
      case 29:
        return second::s29;
      case 30:
        return second::s30;
      case 31:
        return second::s31;
      case 32:
        return second::s32;
      case 33:
        return second::s33;
      case 34:
        return second::s34;
      case 35:
        return second::s35;
      case 36:
        return second::s36;
      case 37:
        return second::s37;
      case 38:
        return second::s38;
      case 39:
        return second::s39;
      case 40:
        return second::s40;
      case 41:
        return second::s41;
      case 42:
        return second::s42;
      case 43:
        return second::s43;
      case 44:
        return second::s44;
      case 45:
        return second::s45;
      case 46:
        return second::s46;
      case 47:
        return second::s47;
      case 48:
        return second::s48;
      case 49:
        return second::s49;
      case 50:
        return second::s50;
      case 51:
        return second::s51;
      case 52:
        return second::s52;
      case 53:
        return second::s53;
      case 54:
        return second::s54;
      case 55:
        return second::s55;
      case 56:
        return second::s56;
      case 57:
        return second::s57;
      case 58:
        return second::s58;
      default:
        return second::s59;
    }
  }

  inline bool operator>(const second& p_second) const
  {
    return m_value > p_second.m_value;
  }

  inline bool operator<(const second& p_second) const
  {
    return m_value < p_second.m_value;
  }

  inline bool operator>=(const second& p_second) const
  {
    return m_value >= p_second.m_value;
  }

  inline bool operator<=(const second& p_second) const
  {
    return m_value <= p_second.m_value;
  }

  inline bool operator==(const second& p_second) const
  {
    return m_value == p_second.m_value;
  }

  inline bool operator!=(const second& p_second) const
  {
    return m_value != p_second.m_value;
  }

  inline operator amount<second>() const { return amount<second>(m_value); }

private:
  inline explicit second(uint8_t p_value)
    : m_value(p_value)
  {}

private:
  uint8_t m_value;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // SECOND_H
