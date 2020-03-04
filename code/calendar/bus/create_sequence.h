#ifndef TENACITAS_CALENDAR_BUS_CREATE_SEQUENCE_H
#define TENACITAS_CALENDAR_BUS_CREATE_SEQUENCE_H

#include <vector>

#include <calendar/bus/weekly_repetition.h>
#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace bus {

///
/// \brief creates a sequence o \p timestamp objects based on a initial \p
/// timestamp, on a repetition pattern, an on an ending condition
///
/// \tparam t_time_precision defines the precision of the timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///
/// \tparam t_repetition defines the repetition pattern. The class must define
/// \code timestamp next(timestamp
/// p_time, bool p_first = false) \endcode
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement \code bool stop(timestamp p_time) \endcode.
///
template<
         typename t_repetition,
         typename t_ending_type>
std::vector<ent::timestamp>
create_sequence(ent::timestamp p_start,
                t_repetition&& p_repetition,
                t_ending_type&& p_end)
{
  std::vector<ent::timestamp> _sequence;

  ent::timestamp _time = p_repetition.next(p_start, true);
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
/// \tparam t_time_precision defines the precision of the timestamp. Currently
/// it can be \p second, \p minute, p weekday, \p hour,\p day, \p month and \p
/// year
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement \code bool stop(timestamp p_time) \endcode.
///
template<typename t_ending_type>
std::vector<ent::timestamp>
create_sequence(ent::timestamp p_start,
                weekly_repetition&& p_repetition,
                t_ending_type&& p_end)

{
  typedef ent::timestamp timestamp;
  typedef std::vector<timestamp> timestamps;

  timestamps _sequence;

  timestamps _times = p_repetition.next(p_start, true);
  while (!p_end.stop(_times.back())) {
    _sequence.insert(_sequence.end(), _times.begin(), _times.end());
    _times = p_repetition.next(_times.back() + ent::amount<ent::day>(1));
  }
  return _sequence;
}

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // CREATE_SEQUENCE_H
