#ifndef TENACITAS_CALENDAR_GREGORIAN_CREATE_SEQUENCE_H
#define TENACITAS_CALENDAR_GREGORIAN_CREATE_SEQUENCE_H

#include <vector>

#include <calendar/_bus/weekly_repetition.h>
#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

///
/// \brief creates a sequence o \p timestamp objects based on a initial \p
/// timestamp, on a repetition pattern, an on an ending condition
///
/// \tparam t_timestamp
///
/// \tparam t_repetition defines the repetition pattern. The class must define
///
/// \code timestamp next(timestamp p_time, bool p_first = false) \endcode
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement
///
/// \code bool stop(timestamp p_time) \endcode.
///
template <typename t_timestamp, typename t_repetition, typename t_ending_type>
std::vector<t_timestamp> create_sequence(t_timestamp p_start,
                                         t_repetition &&p_repetition,
                                         t_ending_type &&p_end) {
  std::vector<t_timestamp> _sequence;

  t_timestamp _time = p_repetition.next(p_start, true);
  while (!p_end.stop(_time)) {
    _sequence.push_back(_time);
    _time = p_repetition.next(_time);
  }
  return _sequence;
}

///
/// \brief creates a sequence o \p timestamp objects based on a initial \p
/// timestamp, on a \p weekly_repetition pattern, an on an ending condition
///
/// \tparam t_timestamp
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement \code bool stop(timestamp p_time) \endcode.
///
template <typename t_timestamp, typename t_ending_type>
std::vector<t_timestamp>
create_sequence(t_timestamp p_start,
                weekly_repetition_t<t_timestamp> &&p_repetition,
                t_ending_type &&p_end)

{
  typedef t_timestamp timestamp;
  typedef std::vector<timestamp> timestamps;

  timestamps _sequence;

  timestamps _times = p_repetition.next(p_start, true);
  while (!p_end.stop(_times.back())) {
    _sequence.insert(_sequence.end(), _times.begin(), _times.end());
    timestamp _next = _times.back() /*+ _ent::days(1)*/;
    _times = p_repetition.next(_next);
  }
  return _sequence;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // CREATE_SEQUENCE_H
