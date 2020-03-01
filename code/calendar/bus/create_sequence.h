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

template<typename t_time_precision,
         typename t_repetition,
         typename t_ending_type>
std::vector<ent::timestamp_t<t_time_precision>>
create_sequence(ent::timestamp_t<t_time_precision> p_start,
                t_repetition&& p_repetition,
                t_ending_type&& p_end)
{
  std::vector<ent::timestamp_t<t_time_precision>> _sequence;

  ent::timestamp_t<t_time_precision> _time = p_repetition.next(p_start, true);
  while (!p_end.stop(_time)) {
    _sequence.push_back(_time);
    _time = p_repetition.next(_time);
  }
  return _sequence;
}

// template<typename t_time_precision, typename t_ending_type>
// std::vector<timestamp_t<t_time_precision>>
// create_sequence
//  (timestamp_<t_time_precision> p_start,

//   weekly_repetition<t_time_precision>&& p_repetition,

//   t_ending_type&& p_end)

template<typename t_time_precision, typename t_ending_type>
std::vector<ent::timestamp_t<t_time_precision>>
create_sequence(ent::timestamp_t<t_time_precision> p_start,
                weekly_repetition<t_time_precision>&& p_repetition,
                t_ending_type&& p_end)

{
  typedef ent::timestamp_t<t_time_precision> timestamp;
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
