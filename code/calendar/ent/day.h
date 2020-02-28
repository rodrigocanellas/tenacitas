#ifndef TENACITAS_CALENDAR_ENTITIES_DAY_H
#define TENACITAS_CALENDAR_ENTITIES_DAY_H

#include <cstdint>
#include <iostream>

#include <calendar/ent/amount.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/second.h>

namespace tenacitas {
namespace calendar {
namespace ent {

/**
 * @brief The day struct
 */
struct day
{
  friend struct amount<day>;

  inline friend std::ostream& operator<<(std::ostream& p_out, const day& p_day)
  {
    p_out << static_cast<uint32_t>(p_day.m_value);
    return p_out;
  }

  static const day d01;
  static const day d02;
  static const day d03;
  static const day d04;
  static const day d05;
  static const day d06;
  static const day d07;
  static const day d08;
  static const day d09;
  static const day d10;
  static const day d11;
  static const day d12;
  static const day d13;
  static const day d14;
  static const day d15;
  static const day d16;
  static const day d17;
  static const day d18;
  static const day d19;
  static const day d20;
  static const day d21;
  static const day d22;
  static const day d23;
  static const day d24;
  static const day d25;
  static const day d26;
  static const day d27;
  static const day d28;
  static const day d29;
  static const day d30;
  static const day d31;

  template<typename t_int>
  static day create(t_int p_int)
  {
    switch (p_int) {
      case 2:
        return d02;
      case 3:
        return d03;
      case 4:
        return d04;
      case 5:
        return d05;
      case 6:
        return d06;
      case 7:
        return d07;
      case 8:
        return d08;
      case 9:
        return d09;
      case 10:
        return d10;
      case 11:
        return d11;
      case 12:
        return d12;
      case 13:
        return d13;
      case 14:
        return d14;
      case 15:
        return d15;
      case 16:
        return d16;
      case 17:
        return d17;
      case 18:
        return d18;
      case 19:
        return d19;
      case 20:
        return d20;
      case 21:
        return d21;
      case 22:
        return d22;
      case 23:
        return d23;
      case 24:
        return d24;
      case 25:
        return d25;
      case 26:
        return d26;
      case 27:
        return d27;
      case 28:
        return d28;
      case 29:
        return d29;
      case 30:
        return d30;
      case 31:
        return d31;
      default:
        return d01;
    }
  }

  inline bool operator>(const day& p_day) const
  {
    return m_value > p_day.m_value;
  }

  inline bool operator<(const day& p_day) const
  {
    return m_value < p_day.m_value;
  }

  inline bool operator>=(const day& p_day) const
  {
    return m_value >= p_day.m_value;
  }

  inline bool operator<=(const day& p_day) const
  {
    return m_value <= p_day.m_value;
  }

  inline bool operator==(const day& p_day) const
  {
    return m_value == p_day.m_value;
  }

  inline bool operator!=(const day& p_day) const
  {
    return m_value != p_day.m_value;
  }

  inline operator amount<day>() const { return amount<day>(m_value); }

  static inline amount<hour> hours() { return amount<hour>(24); }

  static inline amount<minute> minutes() { return hour::minutes() * hours(); }

  static inline amount<second> seconds()
  {
    return minute::seconds() * minutes();
  }

private:
  inline explicit day(uint8_t p_value)
    : m_value(p_value)
  {}

private:
  uint8_t m_value;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // DAY_H
