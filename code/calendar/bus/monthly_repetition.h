#ifndef TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H
#define TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H

#include <cstdint>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/month.h>
#include <calendar/ent/unix.h>
#include <calendar/ent/weekday.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next t_timestamp, given a weekday, the order of the
/// weekday in a month, and the interval between the months
///
/// \tparam t_time_precision defines the precision of the t_timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///
template<typename t_timestamp>
struct monthly_repetition_by_week_t
{

  ///
  /// \brief monthly_repetition_by_week_t
  /// \param p_order
  /// \param p_weekday
  /// \param p_at_each
  ///
  monthly_repetition_by_week_t(uint8_t p_order,
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
  t_timestamp next(t_timestamp p_time, bool p_first = false)
  {
    t_timestamp _timestamp = p_time;
    if (!p_first) {
      _timestamp += m_at_each;
    }

    t_timestamp _day_one(_timestamp);
    _day_one.set_day(ent::day::d01);

    t_timestamp _first_target_weekday(_day_one);
    _first_target_weekday +=
      ent::amount<ent::day>(m_weekday - _day_one.get_weekday());

    t_timestamp _target(_first_target_weekday);

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

///
///
///
template<typename t_timestamp>
struct monthly_repetition_by_day_t
{
  monthly_repetition_by_day_t(ent::day p_day, uint16_t p_at_each = 1)
    : m_day(p_day)
    , m_at_each(p_at_each)
  {
  }

  t_timestamp next(t_timestamp p_time, bool p_first = false)
  {

    t_timestamp _time = p_time;

    ent::day _day(m_day);

    ent::amount<ent::day> _days (ent::month::days(_time.get_month(), _time.get_year()));

    if (ent::amount<ent::day>(_day) > _days) {
      _day = ent::day::create(_days.get<uint8_t>());
    }

    _time.set_day(_day);

    if (!p_first) {
      _time += ent::amount<ent::month>(m_at_each);
    }
    return _time;
  }

private:
  ent::day m_day;
  ent::amount<ent::month> m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // MONTHLY_REPETITION_H
