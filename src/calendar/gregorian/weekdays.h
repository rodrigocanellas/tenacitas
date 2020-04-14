#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKDAYS_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKDAYS_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/weekday.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<int16_t, weekday> weekdays;

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // WEEKDAYS_H
