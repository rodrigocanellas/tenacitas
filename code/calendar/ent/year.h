#ifndef TENACITAS_CALENDAR_ENTITIES_YEAR_H
#define TENACITAS_CALENDAR_ENTITIES_YEAR_H

#include <cstdint>
#include <iostream>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/month.h>
#include <calendar/ent/second.h>

namespace tenacitas {
namespace calendar {
namespace ent {

///
/// \brief The year struct represents a year
///
struct year
{

  friend struct amount<year>;

  template<typename t_int>
  inline explicit year(t_int p_value = 1900)
    : m_value(static_cast<decltype(m_value)>(p_value))
  {}

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const year& p_year)
  {
    p_out << static_cast<uint32_t>(p_year.m_value);
    return p_out;
  }

  inline bool is_leap() const { return (m_value % 4) == 0; }

  inline bool operator>(const year& p_year) const
  {
    return m_value > p_year.m_value;
  }

  inline bool operator<(const year& p_year) const
  {
    return m_value < p_year.m_value;
  }

  inline bool operator>=(const year& p_year) const
  {
    return m_value >= p_year.m_value;
  }

  inline bool operator<=(const year& p_year) const
  {
    return m_value <= p_year.m_value;
  }

  inline bool operator==(const year& p_year) const
  {
    return m_value == p_year.m_value;
  }

  inline bool operator!=(const year& p_year) const
  {
    return m_value != p_year.m_value;
  }

  inline operator amount<year>() const { return amount<year>(m_value); }

  static inline amount<month> months() { return amount<month>(12); }

  static inline amount<day> days(const year& p_year)
  {
    return month::jan.days(month::jan, p_year) +
           month::feb.days(month::feb, p_year) +
           month::mar.days(month::mar, p_year) +
           month::apr.days(month::apr, p_year) +
           month::may.days(month::may, p_year) +
           month::jun.days(month::jun, p_year) +
           month::jul.days(month::jul, p_year) +
           month::ago.days(month::ago, p_year) +
           month::sep.days(month::sep, p_year) +
           month::oct.days(month::oct, p_year) +
           month::nov.days(month::nov, p_year) +
           month::dec.days(month::dec, p_year);
  }

  static inline amount<hour> hours(const year& p_year)
  {
    return month::jan.hours(month::jan, p_year) +
           month::feb.hours(month::feb, p_year) +
           month::mar.hours(month::mar, p_year) +
           month::apr.hours(month::apr, p_year) +
           month::may.hours(month::may, p_year) +
           month::jun.hours(month::jun, p_year) +
           month::jul.hours(month::jul, p_year) +
           month::ago.hours(month::ago, p_year) +
           month::sep.hours(month::sep, p_year) +
           month::oct.hours(month::oct, p_year) +
           month::nov.hours(month::nov, p_year) +
           month::dec.hours(month::dec, p_year);
  }

  static inline amount<minute> minutes(const year& p_year)
  {
    return month::jan.minutes(month::jan, p_year) +
           month::feb.minutes(month::feb, p_year) +
           month::mar.minutes(month::mar, p_year) +
           month::apr.minutes(month::apr, p_year) +
           month::may.minutes(month::may, p_year) +
           month::jun.minutes(month::jun, p_year) +
           month::jul.minutes(month::jul, p_year) +
           month::ago.minutes(month::ago, p_year) +
           month::sep.minutes(month::sep, p_year) +
           month::oct.minutes(month::oct, p_year) +
           month::nov.minutes(month::nov, p_year) +
           month::dec.minutes(month::dec, p_year);
  }

  static inline amount<second> seconds(const year& p_year)
  {
    return month::jan.seconds(month::jan, p_year) +
           month::feb.seconds(month::feb, p_year) +
           month::mar.seconds(month::mar, p_year) +
           month::apr.seconds(month::apr, p_year) +
           month::may.seconds(month::may, p_year) +
           month::jun.seconds(month::jun, p_year) +
           month::jul.seconds(month::jul, p_year) +
           month::ago.seconds(month::ago, p_year) +
           month::sep.seconds(month::sep, p_year) +
           month::oct.seconds(month::oct, p_year) +
           month::nov.seconds(month::nov, p_year) +
           month::dec.seconds(month::dec, p_year);
  }

private:
  uint16_t m_value;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // YEAR_H
