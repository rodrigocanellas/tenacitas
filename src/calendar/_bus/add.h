#ifndef TENACITAS_CALENDAR_BUS_ADD_H
#define TENACITAS_CALENDAR_BUS_ADD_H

#include <calendar/_ent/days.h>

namespace tenacitas {
namespace calendar {
namespace _bus {

template <typename t_timestamp>
t_timestamp add_days(t_timestamp p_timestamp, _ent::days p_days) {
  _ent::days _days = p_timestamp;
  _days += p_days;
  return t_timestamp(_days);
};

} // namespace _bus
} // namespace calendar
} // namespace tenacitas

#endif // ADD_H
