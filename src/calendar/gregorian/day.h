#ifndef TENACITAS_CALENDAR_GREGORIAN_DAY_H
#define TENACITAS_CALENDAR_GREGORIAN_DAY_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The day struct represents a valid day value
///
struct day {

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const day &p_day) {
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

  template <typename t_int> static constexpr day create(t_int p_int) {
    switch (static_cast<decltype(m_value)>(p_int)) {
    case 1:
      return d01;
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
      throw std::runtime_error(std::to_string(p_int) +
                               " is not a valid second value");
    }
  }

  constexpr inline bool operator>(const day &p_day) const {
    return m_value > p_day.m_value;
  }

  constexpr inline bool operator<(const day &p_day) const {
    return m_value < p_day.m_value;
  }

  constexpr inline bool operator>=(const day &p_day) const {
    return m_value >= p_day.m_value;
  }

  constexpr inline bool operator<=(const day &p_day) const {
    return m_value <= p_day.m_value;
  }

  constexpr inline bool operator==(const day &p_day) const {
    return m_value == p_day.m_value;
  }

  constexpr inline bool operator!=(const day &p_day) const {
    return m_value != p_day.m_value;
  }

  //  static inline hours get_hours() { return hours(24); }

  //  static inline minutes get_minutes() {
  //    return hour::get_minutes() * get_hours();
  //  }

  //  static inline seconds get_seconds() {
  //    return minute::get_seconds() * get_minutes();
  //  }

private:
  constexpr inline uint8_t value() const { return m_value; }

  constexpr inline explicit day(uint8_t p_value) : m_value(p_value) {}

private:
  uint8_t m_value;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // DAY_H