
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

const month month::jan(1);
const month month::feb(2);
const month month::mar(3);
const month month::apr(4);
const month month::may(5);
const month month::jun(6);
const month month::jul(7);
const month month::ago(8);
const month month::sep(9);
const month month::oct(10);
const month month::nov(11);
const month month::dec(12);

std::ostream &operator<<(std::ostream &p_out, const month &p_month) {

  if (p_month == month::jan) {
    p_out << "jan";
  } else if (p_month == month::feb) {
    p_out << "feb";
  } else if (p_month == month::mar) {
    p_out << "mar";
  } else if (p_month == month::apr) {
    p_out << "apr";
  } else if (p_month == month::may) {
    p_out << "may";
  } else if (p_month == month::jun) {
    p_out << "jun";
  } else if (p_month == month::jul) {
    p_out << "jul";
  } else if (p_month == month::ago) {
    p_out << "ago";
  } else if (p_month == month::sep) {
    p_out << "sep";
  } else if (p_month == month::oct) {
    p_out << "oct";
  } else if (p_month == month::nov) {
    p_out << "nov";
  } else if (p_month == month::dec) {
    p_out << "dec";
  }
  return p_out;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
