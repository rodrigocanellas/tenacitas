#ifndef TENACITAS_CALENDAR_GREGORIAN_MONTH_H
#define TENACITAS_CALENDAR_GREGORIAN_MONTH_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

struct year;

///
/// \brief The month struct represents a valid month value
///
struct month {
  //  typedef  amount_t<month> amount;

  month() = delete;

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const month &p_month) {
    p_out << static_cast<uint32_t>(p_month.m_value + 1);
    return p_out;
  }

  static const month jan;
  static const month feb;
  static const month mar;
  static const month apr;
  static const month may;
  static const month jun;
  static const month jul;
  static const month ago;
  static const month sep;
  static const month oct;
  static const month nov;
  static const month dec;

  template <typename t_int> static constexpr month create(t_int p_value) {
    switch (static_cast<decltype(m_value)>(p_value)) {
    case 1:
      return jan;
    case 2:
      return feb;
    case 3:
      return mar;
    case 4:
      return apr;
    case 5:
      return may;
    case 6:
      return jun;
    case 7:
      return jul;
    case 8:
      return ago;
    case 9:
      return sep;
    case 10:
      return oct;
    case 11:
      return nov;
    case 12:
      return dec;
    default:
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid number for month");
    }
  }

  inline bool operator>(const month &p_month) const {
    return m_value > p_month.m_value;
  }

  inline bool operator<(const month &p_month) const {
    return m_value < p_month.m_value;
  }

  inline bool operator>=(const month &p_month) const {
    return m_value >= p_month.m_value;
  }

  inline bool operator<=(const month &p_month) const {
    return m_value <= p_month.m_value;
  }

  inline bool operator==(const month &p_month) const {
    return m_value == p_month.m_value;
  }

  inline bool operator!=(const month &p_month) const {
    return m_value != p_month.m_value;
  }

  static days get_days(const month &p_month, const year &p_year);

  static hours get_hours(const month &p_month, const year &p_year);

  static minutes get_minutes(const month &p_month, const year &p_year);

  static seconds get_seconds(const month &p_month, const year &p_year);

private:
  inline uint8_t value() const { return m_value; }

  inline explicit month(uint8_t p_value) : m_value(p_value) {}

private:
  uint8_t m_value;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MONTH_H
