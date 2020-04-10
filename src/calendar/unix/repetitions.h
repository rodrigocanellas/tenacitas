#ifndef TENACITAS_CALENDAR_UNIX_REPETITIONS_H
#define TENACITAS_CALENDAR_UNIX_REPETITIONS_H

#include <calendar/unix/timestamp.h>
#include <calendar/_bus/daily_repetition.h>
#include <calendar/_bus/weekly_repetition.h>
#include <calendar/_bus/monthly_repetition.h>
#include <calendar/_bus/yearly_repetition.h>


namespace tenacitas {
namespace calendar {
namespace unix {
using namespace tenacitas::calendar::_bus;

typedef daily_repetition_t<timestamp> daily_repetition;
typedef monthly_repetition_by_day_t<timestamp> monthly_repetition_by_day;
typedef monthly_repetition_by_week_t<timestamp> monthly_repetition_by_week;
typedef weekly_repetition_t<timestamp> weekly_repetition;
typedef yearly_repetition_t<timestamp> yearly_repetition;

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // REPETITIONS_H
