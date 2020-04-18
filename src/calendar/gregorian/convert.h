#ifndef TENACITAS_CALENDAR_GREGORIAN_CONVERT_H
#define TENACITAS_CALENDAR_GREGORIAN_CONVERT_H

#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

template <typename t_from, typename t_to>
constexpr t_to convert(const t_from &p_from = t_from(1));

template <typename minutes, typename seconds>
constexpr seconds convert(const minutes &p_minutes) {
  return seconds(p_minutes * 60);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // CONVERT_H
