#ifndef TENACITAS_CALENDAR_BUSINESS_CONVERSIONS_H
#define TENACITAS_CALENDAR_BUSINESS_CONVERSIONS_H

#include <cstdint>

namespace tenacitas {
namespace calendar {
namespace bus {

inline int64_t sec2mil(int64_t p_secs) { return p_secs * 1000; }

inline int64_t min2sec(int64_t p_min) { return p_min * 60; }

inline int64_t min2mil(int64_t p_min) { return sec2mil(min2sec(p_min)); }

inline int64_t hr2min(int64_t p_hr) { return p_hr * 60; }

inline int64_t hr2sec(int64_t p_hr) { return min2sec(hr2min(p_hr)); }

inline int64_t hr2mil(int64_t p_hr) { return sec2mil(min2sec(hr2min(p_hr))); }

inline int64_t day2hr(int64_t p_day) { return p_day * 24; }

inline int64_t day2min(int64_t p_day) { return hr2min(day2hr(p_day)); }

inline int64_t day2sec(int64_t p_day) { return min2sec(hr2min(day2hr(p_day))); }

inline int64_t day2mil(int64_t p_day) {
  return sec2mil(min2sec(hr2min(day2hr(p_day))));
}

} // namespace bus
} // namespace calendar
} // namespace tenacitas
#endif // TENACITAS_CALENDAR_CONVERSIONS_H
