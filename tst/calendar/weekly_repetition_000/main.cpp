
#include <cstdint>
#include <iostream>

#include <calendar/_bus/create_sequence.h>
#include <calendar/_bus/ending_after.h>
#include <calendar/_bus/weekly_repetition.h>
#include <calendar/_ent/day.h>
#include <calendar/_ent/month.h>
#include <calendar/_ent/weekday.h>
#include <calendar/_ent/weekdays.h>
#include <calendar/_ent/year.h>
#include <calendar/unix/timestamp.h>

using namespace tenacitas::calendar;

int main() {
  typedef unix::timestamp timestamp;
  typedef _bus::weekly_repetition_t<unix::timestamp> weekly_repetition;
  typedef _bus::end_after_t<timestamp> end_after;
  typedef std::vector<timestamp> timestamps;

  std::cout << "\nweekly_repetition on mon and wed - end_after 10" << std::endl;

  //  timestamp _timesamp(_ent::year(2020),_ent::month::mar,_ent::day::d09);
  timestamp _timestamp;
  _timestamp.set_day(_ent::day::d10);

  std::cout << "starting in = " << _timestamp << std::endl;

  timestamps _sequence = _bus::create_sequence(
      timestamp(), weekly_repetition({_ent::weekday::mon, _ent::weekday::wed}),
      end_after(10));
  for (timestamp _timestamp : _sequence) {
    std::cout << _timestamp << std::endl;
  }
}
