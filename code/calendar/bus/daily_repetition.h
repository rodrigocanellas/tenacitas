#ifndef TENACITAS_CALENDAR_BUS_DAILY_REPETITION_H
#define TENACITAS_CALENDAR_BUS_DAILY_REPETITION_H

#include <cstdint>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next and previous timestamp, given an amount of days
///
template<typename t_time_precision>
struct daily_repetition
{
  typedef ent::timestamp_t<t_time_precision> timestamp;

  ///
  /// \brief daily_repetition
  /// \param p_at_each the amount of days between a timestamp and the next, or
  /// previous
  ///
  inline daily_repetition(uint16_t p_at_each = 1)
    : m_at_each(p_at_each)
  {}

  ///
  /// \brief prev calculates the previous timestamp to a given one
  ///
  /// \param p_time timestamp from each the previous timestamp should be
  /// calculated
  ///
  /// \param p_first if @p true, the timestamp returned is @p p_time
  /// itself; otherwise, it actually calculates the previous timstamp
  ///
  /// \return the previous timestamp to @p p_time, or @p p_time, if @p p_first
  /// is true
  ///
  inline timestamp prev(timestamp p_time, bool p_first = false)
  {
    return (p_first ? p_time : p_time - m_at_each);
  }

  ///
  /// \brief prev calculates the next timestamp to a given one
  ///
  /// \param p_time timestamp from each the next timestamp should be
  /// calculated
  ///
  /// \param p_first if @p true, the timestamp returned is @p p_time
  /// itself; otherwise, it actually calculates the next timestamp
  ///
  /// \return the next timestamp to @p p_time, or @p p_time, if @p p_first
  /// is true
  ///
  inline timestamp next(timestamp p_time, bool p_first = false)
  {
    return (p_first ? p_time : p_time + m_at_each);
  }

private:
  ///
  /// \brief m_at_each amount of days for the next, or previous, timestamp
  ///
  ent::amount<ent::day> m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // DAILY_REPETITION_H
