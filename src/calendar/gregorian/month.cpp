
#include <cstdint>
#include <iostream>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

const month month::jan(0);
const month month::feb(1);
const month month::mar(2);
const month month::apr(3);
const month month::may(4);
const month month::jun(5);
const month month::jul(6);
const month month::ago(7);
const month month::sep(8);
const month month::oct(9);
const month month::nov(10);
const month month::dec(11);

// days
// month::get_days(const month& p_month, const year& p_year)
//{
//  if (p_month == month::jan) {
//    return days(31);
//  }

//  if (p_month == month::feb) {
//    return (p_year.is_leap() ? days(29) : days(28));
//  }

//  if (p_month == month::mar) {
//    return days(31);
//  }

//  if (p_month == month::apr) {
//    return days(30);
//  }

//  if (p_month == month::may) {
//    return days(31);
//  }
//  if (p_month == month::jun) {
//    return days(30);
//  }

//  if (p_month == month::jul) {
//    return days(31);
//  }

//  if (p_month == month::ago) {
//    return days(31);
//  }

//  if (p_month == month::sep) {
//    return days(30);
//  }

//  if (p_month == month::oct) {
//    return days(31);
//  }

//  if (p_month == month::nov) {
//    return days(30);
//  }

//  return days(31);
//}

// hours
// month::get_hours(const month& p_month, const year& p_year)
//{
//  return day::get_hours() * get_days(p_month, p_year);
//}

// minutes
// month::get_minutes(const month& p_month, const year& p_year)
//{
//  return hour::get_minutes() * get_hours(p_month, p_year);
//}

// seconds
// month::get_seconds(const month& p_month, const year& p_year)
//{
//  return minute::get_seconds() * get_minutes(p_month, p_year);
//}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
