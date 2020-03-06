#ifndef TENACITAS_CALENDAR_BUS_DAILY_REPETITION_T_H
#define TENACITAS_CALENDAR_BUS_DAILY_REPETITION_T_H

#include <cstdint>

#include <calendar/ent/day.h>
#include <calendar/ent/days.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next and previous timestamp, given an amount of days
///
template <typename t_timestamp>
struct daily_repetition_t
{
  ///
  /// \brief daily_repetition_t
  /// \param p_at_each the amount of days between a timestamp and the next, or
  /// previous
  ///
  inline daily_repetition_t(uint16_t p_at_each = 1)
    : m_at_each(p_at_each)
  {}

  ///
  /// \brief prev calculates the previous timestamp to a given one
  ///
  /// \param p_time timestamp from each the previous timestamp should be
  /// calculated
  ///
  /// \param p_first if \p true, the timestamp returned is \p p_time
  /// itself; otherwise, it actually calculates the previous timstamp
  ///
  /// \return the previous timestamp to \p p_time, or \p p_time, if \p p_first
  /// is true
  ///
  inline t_timestamp prev(t_timestamp p_time, bool p_first = false)
  {
    return (p_first ? p_time : p_time - m_at_each);
  }

  ///
  /// \brief prev calculates the next timestamp to a given one
  ///
  /// \param p_time timestamp from each the next timestamp should be
  /// calculated
  ///
  /// \param p_first if \p true, the timestamp returned is \p p_time
  /// itself; otherwise, it actually calculates the next timestamp
  ///
  /// \return the next timestamp to \p p_time, or \p p_time, if \p p_first
  /// is true
  ///
  inline t_timestamp next(t_timestamp p_time, bool p_first = false)
  {
    return (p_first ? p_time : p_time + m_at_each);
  }

private:
  ///
  /// \brief m_at_each amount of days for the next, or previous, timestamp
  ///
  ent::days m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // daily_repetition_t_H
