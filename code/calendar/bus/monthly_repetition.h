#ifndef TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H
#define TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H

#include <cstdint>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/month.h>
#include <calendar/ent/timestamp.h>
#include <calendar/ent/weekday.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next timestamp, given a weekday, the order of the
/// weekday in a month, and the interval between the months
///
/// \tparam t_time_precision defines the precision of the timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///
template<typename t_time_precision>
struct monthly_repetition_by_week
{

  typedef ent::timestamp_t<t_time_precision> timestamp;

  ///
  /// \brief monthly_repetition_by_week
  /// \param p_order
  /// \param p_weekday
  /// \param p_at_each
  ///
  monthly_repetition_by_week(uint8_t p_order,
                             ent::weekday p_weekday,
                             uint16_t p_at_each = 1)
    : m_order(p_order)
    , m_weekday(p_weekday)
    , m_at_each(p_at_each)
  {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  timestamp next(timestamp p_time, bool p_first = false)
  {
    timestamp _timestamp = p_time;
    if (!p_first) {
      _timestamp += m_at_each;
    }

    timestamp _day_one(_timestamp);
    _day_one.set_day(ent::day::d01);

    timestamp _first_target_weekday(_day_one);
    _first_target_weekday +=
      ent::amount<ent::day>(m_weekday - _day_one.get_weekday());

    timestamp _target(_first_target_weekday);

    _target += ent::amount<ent::weekday>(m_order - 1);

    return _target;
  }

private:
  ///
  /// \brief m_order
  ///
  uint8_t m_order;

  ///
  /// \brief m_weekday
  ///
  ent::weekday m_weekday;

  ///
  /// \brief m_at_each
  ///
  ent::amount<ent::month> m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // MONTHLY_REPETITION_H
