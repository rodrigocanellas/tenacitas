#ifndef TENACITAS_CALENDAR_GREGORIAN_DAYS_H
#define TENACITAS_CALENDAR_GREGORIAN_DAYS_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/day.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<int32_t, day> days;

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // DAYS_H
