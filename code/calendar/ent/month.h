#ifndef TENACITAS_CALENDAR_ENT_MONTH_H
#define TENACITAS_CALENDAR_ENT_MONTH_H

#include <cstdint>
#include <iostream>


#include <calendar/ent/day.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/second.h>
#include <calendar/ent/days.h>
#include <calendar/ent/hours.h>
#include <calendar/ent/minutes.h>
#include <calendar/ent/seconds.h>


namespace tenacitas {
namespace calendar {
namespace ent {

struct year;

///
/// \brief The month struct represents a valid month value
///
struct month
{
  friend struct amount_t<month>;

//  typedef  amount_t<month> amount;

  month() = delete;

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const month& p_month)
  {
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

  template<typename t_int>
  static month create(t_int p_value)
  {
    switch (static_cast<decltype(m_value)>(p_value)) {
      case 0:
        return jan;
      case 1:
        return feb;
      case 2:
        return mar;
      case 3:
        return apr;
      case 4:
        return may;
      case 5:
        return jun;
      case 6:
        return jul;
      case 7:
        return ago;
      case 8:
        return sep;
      case 9:
        return oct;
      case 10:
        return nov;
      case 11:
        return dec;
      default:
        throw std::runtime_error(std::to_string(p_value) +
                                 " is not a valid number for month");
    }
  }

  inline bool operator>(const month& p_month) const
  {
    return m_value > p_month.m_value;
  }

  inline bool operator<(const month& p_month) const
  {
    return m_value < p_month.m_value;
  }

  inline bool operator>=(const month& p_month) const
  {
    return m_value >= p_month.m_value;
  }

  inline bool operator<=(const month& p_month) const
  {
    return m_value <= p_month.m_value;
  }

  inline bool operator==(const month& p_month) const
  {
    return m_value == p_month.m_value;
  }

  inline bool operator!=(const month& p_month) const
  {
    return m_value != p_month.m_value;
  }



  static days get_days(const month& p_month, const year& p_year);

  static hours get_hours(const month& p_month, const year& p_year);

  static minutes get_minutes(const month& p_month, const year& p_year);

  static seconds get_seconds(const month& p_month, const year& p_year);

private:
  inline uint8_t value() const { return m_value; }

  inline explicit month(uint8_t p_value)
    : m_value(p_value)
  {}

private:
  uint8_t m_value;
};


} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // MONTH_H
