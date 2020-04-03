#ifndef TENACITAS_CALENDAR_UNIX_CREATE_SEQUENCE_H
#define TENACITAS_CALENDAR_UNIX_CREATE_SEQUENCE_H

#include <calendar/unix/repetitions.h>
#include <calendar/_bus/create_sequence.h>
#include <calendar/unix/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace unix {


template<typename t_repetition,
         typename t_ending_type>
std::vector<timestamp>
create_sequence(timestamp p_start,
                t_repetition&& p_repetition,
                t_ending_type&& p_end)
{
  return tenacitas::calendar::_bus::create_sequence(p_start
                                                   , std::move(p_repetition)
                                                   , std::move(p_end));
}


template<typename t_ending_type>
std::vector<timestamp>
create_sequence(timestamp p_start,
                weekly_repetition&& p_repetition,
                t_ending_type&& p_end) {
  return tenacitas::calendar::_bus::create_sequence(p_start
                                                   , std::move(p_repetition)
                                                   , std::move(p_end));

}

} // namespace unix
} // namespace calendar
} // namespace tenacitas


#endif // CREATE_SEQUENCE_H
