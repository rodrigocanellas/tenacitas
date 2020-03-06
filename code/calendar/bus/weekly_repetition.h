#ifndef TENACITAS_CALENDAR_BUS_WEEKLY_REPETITION_T_H
#define TENACITAS_CALENDAR_BUS_WEEKLY_REPETITION_T_H

#include <vector>

#include <calendar/ent/weekday.h>
#include <calendar/ent/weekdays.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief Calculates the next group of timestamps, given a set of \p weekday,
/// and the interval between the weeks
///
/// \tparam t_time_precision defines the precision of the t_timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///

template<typename t_timestamp>
struct weekly_repetition_t
{
  typedef std::vector<t_timestamp> timestamps;

  typedef std::vector<ent::weekday> weekdays;

  ///
  /// \brief weekly_repetition_t
  /// \param p_weekdays
  /// \param p_at_each
  ///
  weekly_repetition_t(weekdays&& p_weekdays, uint16_t p_at_each = 1)
    : m_weekdays(std::move(p_weekdays))
    , m_at_each(p_at_each)
  {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  timestamps next(t_timestamp p_time, bool p_first = false)
  {
    timestamps _sequence;
    if (p_first) {
      first(p_time, _sequence);
      return _sequence;
    }

    t_timestamp _time = begin_week(p_time);
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
  t_timestamp begin_week(t_timestamp p_time)
  {
    t_timestamp _time = p_time + m_at_each;

    _time -= _time.get_weekday().from_sunday();

    return _time;
  }

  ///
  /// \brief next_timestamp_in_weekday_from_timestamp
  /// \param p_time
  /// \param p_weekday
  /// \return
  ///
  t_timestamp next_timestamp_in_weekday_from_timestamp(t_timestamp p_time,
                                                     ent::weekday p_weekday)
  {
    ent::weekday _weekday = p_time.get_weekday();

    t_timestamp _time = p_time;

    if (_weekday == p_weekday) {
      return _time;
    }

    if (_weekday < p_weekday) {
      _time += (p_weekday - _weekday);
    } else {
      //      _time += _weekday.until_saturday();
      //      _time += day::amount(1);
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
            t_timestamp p_time,
            timestamps& p_sequence)
  {
    t_timestamp _time = p_time;
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
  void first(t_timestamp p_time, timestamps& p_sequence)
  {
    t_timestamp _time =
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
  ent::weekdays m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // weekly_repetition_t_H
