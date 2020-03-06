#ifndef TENACITAS_CALENDAR_ENT_WEEKDAY_H
#define TENACITAS_CALENDAR_ENT_WEEKDAY_H

#include <cstdint>
#include <iostream>


#include <calendar/ent/day.h>
#include <calendar/ent/days.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/second.h>

namespace tenacitas {
namespace calendar {
namespace ent {

///
/// \brief The weekday struct represents valid a weekday value
///
struct weekday
{
  friend struct amount_t<weekday>;

//  typedef amount_t<weekday> amount;

  weekday() = delete;

  static const weekday sun;
  static const weekday mon;
  static const weekday tue;
  static const weekday wed;
  static const weekday thu;
  static const weekday fri;
  static const weekday sat;

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const weekday& p_weekday)
  {
    //    if (p_weekday == weekday::sun) {
    //      p_out << 'D';
    //    } else if (p_weekday == weekday::sat) {
    //      p_out << 'S';
    //    } else {
    //      p_out << static_cast<uint32_t>(p_weekday.m_value + 1);
    //    }
    if (p_weekday == weekday::sun) {
      p_out << "sun";
    } else if (p_weekday == weekday::mon) {
      p_out << "mon";
    } else if (p_weekday == weekday::tue) {
      p_out << "tue";
    } else if (p_weekday == weekday::wed) {
      p_out << "wed";
    } else if (p_weekday == weekday::thu) {
      p_out << "thu";
    } else if (p_weekday == weekday::fri) {
      p_out << "fri";
    } else {
      p_out << "sat";
    }

    return p_out;
  }

  template<typename t_int>
  static weekday create(t_int p_value)
  {
    switch (static_cast<decltype(m_value)>(p_value)) {
      case 0:
        return sun;
      case 1:
        return mon;
      case 2:
        return tue;
      case 3:
        return wed;
      case 4:
        return thu;
      case 5:
        return fri;
      case 6:
        return sat;
      default:
        throw std::runtime_error(std::to_string(p_value) +
                                 " is not a valid number for weekday");
    }
  }

  inline bool operator>(const weekday& p_weekday) const
  {
    return m_value > p_weekday.m_value;
  }

  inline bool operator<(const weekday& p_weekday) const
  {
    return m_value < p_weekday.m_value;
  }

  inline bool operator>=(const weekday& p_weekday) const
  {
    return m_value >= p_weekday.m_value;
  }

  inline bool operator<=(const weekday& p_weekday) const
  {
    return m_value <= p_weekday.m_value;
  }

  inline bool operator==(const weekday& p_weekday) const
  {
    return m_value == p_weekday.m_value;
  }

  inline bool operator!=(const weekday& p_weekday) const
  {
    return m_value != p_weekday.m_value;
  }

  static inline days get_days() { return days(7); }

  static inline hours get_hours() { return day::get_hours() * days().get<hours>(); }

  static inline minutes get_minutes() { return hour::get_minutes() * hours().get<minutes>(); }

  static inline seconds get_seconds()
  {
    return minute::get_seconds() * minutes().get<seconds>();
  }

  days operator-(const weekday& p_weekday) const
  {
    if (m_value > p_weekday.m_value) {
      return days(m_value - p_weekday.m_value);
    }
    if (m_value == p_weekday.m_value) {
      return days(0);
    }

    return days((sat.m_value - p_weekday.m_value) +
                       (m_value - sun.m_value + 1));
  }

  inline days from_sunday() { return (*this - weekday::sun); }

  inline days until_saturday() { return (weekday::sat - *this); }

private:


  inline uint8_t value()const { return m_value; }

  inline weekday(uint8_t p_value)
    : m_value(p_value)
  {}

private:
  uint8_t m_value;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // WEEKDAY_H
