#ifndef TENACITAS_CALENDAR_GREGORIAN_HOUR_H
#define TENACITAS_CALENDAR_GREGORIAN_HOUR_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The hour struct represents a valid hour value
///
struct hour {
  //  typedef amount_t<hour> amount;

  hour() = delete;

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const hour &p_hour) {
    p_out << static_cast<uint32_t>(p_hour.m_value);
    return p_out;
  }

  static const hour h00;
  static const hour h01;
  static const hour h02;
  static const hour h03;
  static const hour h04;
  static const hour h05;
  static const hour h06;
  static const hour h07;
  static const hour h08;
  static const hour h09;
  static const hour h10;
  static const hour h11;
  static const hour h12;
  static const hour h13;
  static const hour h14;
  static const hour h15;
  static const hour h16;
  static const hour h17;
  static const hour h18;
  static const hour h19;
  static const hour h20;
  static const hour h21;
  static const hour h22;
  static const hour h23;

  template <typename t_int> constexpr static hour create(t_int p_int) {
    switch (p_int) {
    case 0:
      return hour::h00;
    case 1:
      return hour::h01;
    case 2:
      return hour::h02;
    case 3:
      return hour::h03;
    case 4:
      return hour::h04;
    case 5:
      return hour::h05;
    case 6:
      return hour::h06;
    case 7:
      return hour::h07;
    case 8:
      return hour::h08;
    case 9:
      return hour::h09;
    case 10:
      return hour::h10;
    case 11:
      return hour::h11;
    case 12:
      return hour::h12;
    case 13:
      return hour::h13;
    case 14:
      return hour::h14;
    case 15:
      return hour::h15;
    case 16:
      return hour::h16;
    case 17:
      return hour::h17;
    case 18:
      return hour::h18;
    case 19:
      return hour::h19;
    case 20:
      return hour::h20;
    case 21:
      return hour::h21;
    case 22:
      return hour::h22;
    case 23:
      return hour::h23;
    default:
      throw std::runtime_error(std::to_string(p_int) +
                               " is not a valid hour value");
    }
  }

  constexpr inline bool operator>(const hour &p_hour) const {
    return m_value > p_hour.m_value;
  }

  constexpr inline bool operator<(const hour &p_hour) const {
    return m_value < p_hour.m_value;
  }

  constexpr inline bool operator>=(const hour &p_hour) const {
    return m_value >= p_hour.m_value;
  }

  constexpr inline bool operator<=(const hour &p_hour) const {
    return m_value <= p_hour.m_value;
  }

  constexpr inline bool operator==(const hour &p_hour) const {
    return m_value == p_hour.m_value;
  }

  constexpr inline bool operator!=(const hour &p_hour) const {
    return m_value != p_hour.m_value;
  }

  //  static inline minutes get_minutes() { return minutes(60); }

  //  static inline seconds get_seconds()
  //  {
  //    return minute::get_seconds() * get_minutes();
  //  }

private:
  constexpr inline uint8_t value() const { return m_value; }

  constexpr inline explicit hour(uint8_t p_value) : m_value(p_value) {}

private:
  uint8_t m_value;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // HOUR_H
