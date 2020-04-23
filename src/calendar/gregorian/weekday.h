#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H

#include <cstdint>
#include <iostream>

//#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>
//#include <calendar/gregorian/hour.h>
//#include <calendar/gregorian/minute.h>
//#include <calendar/gregorian/second.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The weekday struct represents valid a weekday value
///
enum class weekday : uint8_t {
  sun = 0,
  mon = 1,
  tue = 2,
  wed = 3,
  thu = 4,
  fri = 5,
  sat = 6
};

///
/// \brief operator - how many days between two weekdays
/// \param p_left left operand
/// \param p_right right operand
/// \return  amount of days betwen the weekdays
///
constexpr days operator-(const weekday &p_left, const weekday &p_right) {
  if (p_left > p_right) {
    return days(static_cast<uint8_t>(p_left) - static_cast<uint8_t>(p_right));
  }
  if (p_left == p_right) {
    return days(0);
  }

  days _d0(weekday::sat - p_right);
  days _d1(p_left - weekday::sun);
  _d1++;
  return days(_d0 - _d1);
}

///
/// \brief operator ++ next weekday
/// \param p_w weekday to be advanced
///
inline constexpr void operator++(weekday &p_w) {
  p_w = (p_w == weekday::sat ? weekday::sun
                             : weekday(static_cast<uint8_t>(p_w) + 1));
}

///
/// \brief operator ++ next weekday
/// \param p_w weekday to be advanced
///
inline constexpr void operator++(weekday &p_w, int) {
  p_w = (p_w == weekday::sat ? weekday::sun
                             : weekday(static_cast<uint8_t>(p_w) + 1));
}

///
/// \brief operator -- previous weekday
/// \param p_w weekday to be go back
///
inline constexpr void operator--(weekday &p_w) {
  p_w = ((p_w == weekday::sun) ? weekday::sat
                               : weekday(static_cast<uint8_t>(p_w) - 1));
}

///
/// \brief operator -- previous weekday
/// \param p_w weekday to be go back
///
constexpr void operator--(weekday &p_w, int) {
  p_w = (p_w == weekday::sun) ? weekday::sat
                              : weekday(static_cast<uint8_t>(p_w) - 1);
}

///
/// \brief operator << prints the weekday
/// \param p_out output stream
/// \param p_weekday weekday to be printed
/// \return output stream
///
inline std::ostream &operator<<(std::ostream &p_out, const weekday &p_weekday) {
  if (p_weekday == weekday::sun) {
    p_out << "sun";
  } else if (p_weekday == weekday::mon) {
    p_out << "mon";
  } else if (p_weekday == weekday::tue) {
    p_out << "tue";
  } else if (p_weekday == weekday::wed) {
    p_out << "wed";
  } else if (p_weekday == weekday::thu) {
    p_out << "thu";
  } else if (p_weekday == weekday::fri) {
    p_out << "fri";
  } else {
    p_out << "sat";
  }

  return p_out;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // WEEKDAY_H
