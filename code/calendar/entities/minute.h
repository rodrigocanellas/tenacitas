#ifndef TENACITAS_CALENDAR_ENTITIES_MINUTE_H
#define TENACITAS_CALENDAR_ENTITIES_MINUTE_H

#include <cstdint>
#include <iostream>

#include <calendar/entities/amount.h>
#include <calendar/entities/second.h>

namespace tenacitas {
namespace calendar {
namespace entities {
/**
 * @brief The minute struct
 */
struct minute
{

  friend struct amount<minute>;

  minute() = delete;

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const minute& p_minute)
  {
    p_out << static_cast<uint32_t>(p_minute.m_value);
    return p_out;
  }

  static const minute m00;
  static const minute m01;
  static const minute m02;
  static const minute m03;
  static const minute m04;
  static const minute m05;
  static const minute m06;
  static const minute m07;
  static const minute m08;
  static const minute m09;
  static const minute m10;
  static const minute m11;
  static const minute m12;
  static const minute m13;
  static const minute m14;
  static const minute m15;
  static const minute m16;
  static const minute m17;
  static const minute m18;
  static const minute m19;
  static const minute m20;
  static const minute m21;
  static const minute m22;
  static const minute m23;
  static const minute m24;
  static const minute m25;
  static const minute m26;
  static const minute m27;
  static const minute m28;
  static const minute m29;
  static const minute m30;
  static const minute m31;
  static const minute m32;
  static const minute m33;
  static const minute m34;
  static const minute m35;
  static const minute m36;
  static const minute m37;
  static const minute m38;
  static const minute m39;
  static const minute m40;
  static const minute m41;
  static const minute m42;
  static const minute m43;
  static const minute m44;
  static const minute m45;
  static const minute m46;
  static const minute m47;
  static const minute m48;
  static const minute m49;
  static const minute m50;
  static const minute m51;
  static const minute m52;
  static const minute m53;
  static const minute m54;
  static const minute m55;
  static const minute m56;
  static const minute m57;
  static const minute m58;
  static const minute m59;

  template<typename t_int>
  static minute create(t_int p_int)
  {
    switch (p_int) {
      case 0:
        return minute::m00;
      case 1:
        return minute::m01;
      case 2:
        return minute::m02;
      case 3:
        return minute::m03;
      case 4:
        return minute::m04;
      case 5:
        return minute::m05;
      case 6:
        return minute::m06;
      case 7:
        return minute::m07;
      case 8:
        return minute::m08;
      case 9:
        return minute::m09;
      case 10:
        return minute::m10;
      case 11:
        return minute::m11;
      case 12:
        return minute::m12;
      case 13:
        return minute::m13;
      case 14:
        return minute::m14;
      case 15:
        return minute::m15;
      case 16:
        return minute::m16;
      case 17:
        return minute::m17;
      case 18:
        return minute::m18;
      case 19:
        return minute::m19;
      case 20:
        return minute::m20;
      case 21:
        return minute::m21;
      case 22:
        return minute::m22;
      case 23:
        return minute::m23;
      case 24:
        return minute::m24;
      case 25:
        return minute::m25;
      case 26:
        return minute::m26;
      case 27:
        return minute::m27;
      case 28:
        return minute::m28;
      case 29:
        return minute::m29;
      case 30:
        return minute::m30;
      case 31:
        return minute::m31;
      case 32:
        return minute::m32;
      case 33:
        return minute::m33;
      case 34:
        return minute::m34;
      case 35:
        return minute::m35;
      case 36:
        return minute::m36;
      case 37:
        return minute::m37;
      case 38:
        return minute::m38;
      case 39:
        return minute::m39;
      case 40:
        return minute::m40;
      case 41:
        return minute::m41;
      case 42:
        return minute::m42;
      case 43:
        return minute::m43;
      case 44:
        return minute::m44;
      case 45:
        return minute::m45;
      case 46:
        return minute::m46;
      case 47:
        return minute::m47;
      case 48:
        return minute::m48;
      case 49:
        return minute::m49;
      case 50:
        return minute::m50;
      case 51:
        return minute::m51;
      case 52:
        return minute::m52;
      case 53:
        return minute::m53;
      case 54:
        return minute::m54;
      case 55:
        return minute::m55;
      case 56:
        return minute::m56;
      case 57:
        return minute::m57;
      case 58:
        return minute::m58;
      default:
        return minute::m59;
    }
  }

  inline bool operator>(const minute& p_minute) const
  {
    return m_value > p_minute.m_value;
  }

  inline bool operator<(const minute& p_minute) const
  {
    return m_value < p_minute.m_value;
  }

  inline bool operator>=(const minute& p_minute) const
  {
    return m_value >= p_minute.m_value;
  }

  inline bool operator<=(const minute& p_minute) const
  {
    return m_value <= p_minute.m_value;
  }

  inline bool operator==(const minute& p_minute) const
  {
    return m_value == p_minute.m_value;
  }

  inline bool operator!=(const minute& p_minute) const
  {
    return m_value != p_minute.m_value;
  }

  inline operator amount<minute>() const { return amount<minute>(m_value); }

  static inline amount<second> seconds() { return amount<second>(60); }

private:
  inline explicit minute(uint8_t p_value)
    : m_value(p_value)
  {}

private:
  uint8_t m_value;
};

} // namespace entities
} // namespace calendar
} // namespace tenacitas

#endif // MINUTE_H
