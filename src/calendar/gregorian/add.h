#ifndef TENACITAS_CALENDAR_GREGORIAN_ADD_H
#define TENACITAS_CALENDAR_GREGORIAN_ADD_H

#include <calendar/gregorian/days.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template <typename t_timestamp>
t_timestamp add_days(t_timestamp p_timestamp, days p_days) {
  days _days = p_timestamp;
  _days += p_days;
  return t_timestamp(_days);
};

} // namespace greogorian
} // namespace calendar
} // namespace tenacitas

#endif // ADD_H
