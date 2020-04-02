#ifndef TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H
#define TENACITAS_CALENDAR_BUS_MONTHLY_REPETITION_H

#include <cstdint>

#include <calendar/ent/day.h>
#include <calendar/ent/days.h>
#include <calendar/ent/month.h>
#include <calendar/ent/months.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/weekdays.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next t_timestamp, given a weekday, the order of the
/// weekday in a month, and the interval between the months
///
/// \tparam needs to be copy constructable
///
template <typename t_timestamp> struct monthly_repetition_by_week_t {

  ///
  /// \brief monthly_repetition_by_week_t
  /// \param p_order
  /// \param p_weekday
  /// \param p_at_each
  ///
  monthly_repetition_by_week_t(uint8_t p_order, ent::weekday p_weekday,
                               uint16_t p_at_each = 1)
      : m_order(p_order), m_weekday(p_weekday), m_at_each(p_at_each) {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  t_timestamp next(t_timestamp p_time, bool p_first = false) {
    t_timestamp _timestamp = p_time;
    if (!p_first) {
      _timestamp += m_at_each;
    }

    t_timestamp _day_one(_timestamp);
    _day_one.set_day(ent::day::d01);

    t_timestamp _first_target_weekday(_day_one);
    _first_target_weekday += ent::days(m_weekday - _day_one.get_weekday());

    t_timestamp _target(_first_target_weekday);

    _target += ent::weekdays(m_order - 1);

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
  ent::months m_at_each;
};

///
///
///
template <typename t_timestamp> struct monthly_repetition_by_day_t {
  monthly_repetition_by_day_t(ent::day p_day, uint16_t p_at_each = 1)
      : m_day(p_day), m_at_each(p_at_each) {}

  t_timestamp next(t_timestamp p_time, bool p_first = false) {

    t_timestamp _time(p_time);

    ent::day _day(m_day);

    ent::days _days(ent::month::get_days(_time.get_month(), _time.get_year()));

    if (ent::days(_day) > _days) {
      _day = ent::day::create(_days.get<uint8_t>());
    }

    _time.set_day(_day);

    if (!p_first) {
      _time += ent::months(m_at_each);
    }
    return _time;
  }

private:
  ent::day m_day;
  ent::months m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // MONTHLY_REPETITION_H
