
#include <iostream>
#include <cstdint>

#include <calendar/ent/day.h>
#include <calendar/ent/month.h>
#include <calendar/ent/year.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/weekdays.h>
#include <calendar/bus/weekly_repetition.h>
#include <calendar/ent/timestamp_unix.h>
#include <calendar/bus/ending_after.h>
#include <calendar/bus/create_sequence.h>

using namespace tenacitas::calendar;

int main() {
  typedef ent::timestamp_unix timestamp;
  typedef bus::weekly_repetition_t<ent::timestamp_unix>  weekly_repetition;
  typedef bus::end_after_t<timestamp> end_after;
  typedef std::vector<timestamp> timestamps;

  std::cout << "\nweekly_repetition on mon and wed - end_after 10" << std::endl;

//  timestamp _timesamp(ent::year(2020),ent::month::mar,ent::day::d09);
    timestamp _timestamp;
    _timestamp.set_day(ent::day::d10);


  std::cout << "starting in = " << _timestamp << std::endl;

  timestamps _sequence =
    bus::create_sequence(timestamp(),
                    weekly_repetition({ ent::weekday::mon, ent::weekday::wed }),
                    end_after(10));
  for (timestamp _timestamp : _sequence) {
    std::cout << _timestamp << std::endl;
  }
}
