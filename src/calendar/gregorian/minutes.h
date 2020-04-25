#ifndef TENACITAS_CALENDAR_GREGORIAN_MINUTES_H
#define TENACITAS_CALENDAR_GREGORIAN_MINUTES_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<minute> minutes;

template <>
constexpr inline seconds convert<minutes, seconds>(const minutes &p_minutes) {
  return seconds(p_minutes * 60);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MINUTES_H
