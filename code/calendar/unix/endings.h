#ifndef TENACITAS_CALENDAR_UNIX_ENDINGS_H
#define TENACITAS_CALENDAR_UNIX_ENDINGS_H

#include <calendar/_bus/ending_after.h>
#include <calendar/_bus/ending_never.h>
#include <calendar/_bus/ending_on.h>
#include <calendar/unix/timestamp.h>


namespace tenacitas {
namespace calendar {
namespace unix {
using namespace tenacitas::calendar::_bus;

typedef end_on_t<timestamp> end_on;
typedef end_after_t<timestamp> end_after;
typedef end_never_t<timestamp> end_never;

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // ENDINGS_H
