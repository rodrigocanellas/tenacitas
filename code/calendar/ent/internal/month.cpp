#include <calendar/entities/month.h>
#include <calendar/entities/year.h>

#include <cstdint>
#include <iostream>

#include <calendar/entities/amount.h>
#include <calendar/entities/day.h>
#include <calendar/entities/hour.h>
#include <calendar/entities/minute.h>
#include <calendar/entities/second.h>

namespace tenacitas {
namespace calendar {
namespace entities {

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

amount<day>
month::days(const month& p_month, const year& p_year)
{
  if (p_month == month::jan) {
    return amount<day>(31);
  }

  if (p_month == month::feb) {
    return (p_year.is_leap() ? amount<day>(29) : amount<day>(28));
  }

  if (p_month == month::mar) {
    return amount<day>(31);
  }

  if (p_month == month::apr) {
    return amount<day>(30);
  }

  if (p_month == month::may) {
    return amount<day>(31);
  }
  if (p_month == month::jun) {
    return amount<day>(30);
  }

  if (p_month == month::jul) {
    return amount<day>(31);
  }

  if (p_month == month::ago) {
    return amount<day>(31);
  }

  if (p_month == month::sep) {
    return amount<day>(30);
  }

  if (p_month == month::oct) {
    return amount<day>(31);
  }

  if (p_month == month::nov) {
    return amount<day>(30);
  }

  return amount<day>(31);
}

amount<hour>
month::hours(const month& p_month, const year& p_year)
{
  return day::hours() * days(p_month, p_year);
}

amount<minute>
month::minutes(const month& p_month, const year& p_year)
{
  return hour::minutes() * hours(p_month, p_year);
}

amount<second>
month::seconds(const month& p_month, const year& p_year)
{
  return minute::seconds() * minutes(p_month, p_year);
}

} // namespace entities
} // namespace calendar
} // namespace tenacitas
