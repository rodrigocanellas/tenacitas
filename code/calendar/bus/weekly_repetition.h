#ifndef TENACITAS_CALENDAR_BUS_WEEKLY_REPETITION_H
#define TENACITAS_CALENDAR_BUS_WEEKLY_REPETITION_H

#include <vector>

#include <calendar/ent/amount.h>
#include <calendar/ent/timestamp.h>
#include <calendar/ent/weekday.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next group of timestamps, given a set of \p weekday,
/// and the interval between the weeks
///
/// \tparam t_time_precision defines the precision of the ent::timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///
struct weekly_repetition
{
  typedef std::vector<ent::timestamp> timestamps;

  typedef std::vector<ent::weekday> weekdays;

  ///
  /// \brief weekly_repetition
  /// \param p_weekdays
  /// \param p_at_each
  ///
  weekly_repetition(weekdays&& p_weekdays, uint16_t p_at_each = 1)
    : m_weekdays(std::move(p_weekdays))
    , m_at_each(p_at_each)
  {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  timestamps next(ent::timestamp p_time, bool p_first = false)
  {
    timestamps _sequence;
    if (p_first) {
      first(p_time, _sequence);
      return _sequence;
    }

    ent::timestamp _time = begin_week(p_time);
    //
    rest(m_weekdays.begin(), _time, _sequence);
    return _sequence;
  }

private:
  ///
  /// \brief begin_week
  /// \param p_time
  /// \return
  ///
  ent::timestamp begin_week(ent::timestamp p_time)
  {
    ent::timestamp _time = p_time + m_at_each;

    _time -= _time.get_weekday().from_sunday();

    return _time;
  }

  ///
  /// \brief next_timestamp_in_weekday_from_timestamp
  /// \param p_time
  /// \param p_weekday
  /// \return
  ///
  ent::timestamp next_timestamp_in_weekday_from_timestamp(ent::timestamp p_time,
                                                     ent::weekday p_weekday)
  {
    ent::weekday _weekday = p_time.get_weekday();

    ent::timestamp _time = p_time;

    if (_weekday == p_weekday) {
      return _time;
    }

    if (_weekday < p_weekday) {
      _time += (p_weekday - _weekday);
    } else {
      //      _time += _weekday.until_saturday();
      //      _time += amount<day>(1);
      //      _time += p_weekday.from_sunday();
      _time += (_weekday - p_weekday);
    }
    return _time;
  }

  ///
  /// \brief rest
  /// \param p_weekday
  /// \param p_time
  /// \param p_sequence
  ///
  void rest(std::vector<ent::weekday>::const_iterator p_weekday,
            ent::timestamp p_time,
            timestamps& p_sequence)
  {
    ent::timestamp _time = p_time;
    for (weekdays::const_iterator _ite = p_weekday; _ite != m_weekdays.end();
         ++_ite) {

      _time += (*_ite - _time.get_weekday());

      p_sequence.push_back(_time);
    }
  }

  ///
  /// \brief first
  /// \param p_time
  /// \param p_sequence
  ///
  void first(ent::timestamp p_time, timestamps& p_sequence)
  {
    ent::timestamp _time =
      next_timestamp_in_weekday_from_timestamp(p_time, m_weekdays[0]);
    p_sequence.push_back(_time);
    rest(++m_weekdays.begin(), _time, p_sequence);
  }

private:
  ///
  /// \brief m_weekdays
  ///
  weekdays m_weekdays;

  ///
  /// \brief m_at_each
  ///
  ent::amount<ent::weekday> m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // WEEKLY_REPETITION_H
