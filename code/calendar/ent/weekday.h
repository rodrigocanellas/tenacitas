#ifndef TENACITAS_CALENDAR_ENTITIES_WEEKDAY_H
#define TENACITAS_CALENDAR_ENTITIES_WEEKDAY_H

#include <cstdint>
#include <iostream>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
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
  friend struct amount<weekday>;

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
    if (p_weekday == weekday::sun) {
      p_out << 'D';
    } else if (p_weekday == weekday::sat) {
      p_out << 'S';
    } else {
      p_out << static_cast<uint32_t>(p_weekday.m_value + 1);
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

  inline operator amount<weekday>() { return amount<weekday>(m_value); }

  static inline amount<day> days() { return amount<day>(7); }

  static inline amount<hour> hours() { return day::hours() * days(); }

  static inline amount<minute> minutes() { return hour::minutes() * hours(); }

  static inline amount<second> seconds()
  {
    return minute::seconds() * minutes();
  }

  amount<day> operator-(const weekday& p_weekday) const
  {
    if (m_value > p_weekday.m_value) {
      return amount<day>(m_value - p_weekday.m_value);
    }
    if (m_value == p_weekday.m_value) {
      return amount<day>(0);
    }

    return amount<day>((sat.m_value - p_weekday.m_value) +
                       (m_value - sun.m_value + 1));
  }

private:
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
