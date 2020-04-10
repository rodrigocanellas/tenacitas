
#include <cstdint>
#include <iostream>
#include <vector>

//

#include <calendar/_ent/days.h>
#include <calendar/_ent/weekday.h>
#include <calendar/unix/create_sequence.h>
#include <calendar/unix/endings.h>
#include <calendar/unix/repetitions.h>
#include <calendar/unix/timestamp.h>

using namespace tenacitas::calendar;

/*
// day of the week for january 1st
#include <iostream>
using namespace std;
int main ()
{
  char*week[]={"sun","mon","tue","wed","thu","fri","sat"};
  int year,days;
  cout<< "year?"; cin>>year;
  days = 365*year + (year-1)/4 - (year-1)/100 + (year-1)/400 ;
  cout<<week[days%7]<<"\n";
  return 0;
}
*/

int main() {

  typedef std::vector<unix::timestamp> timestamps;

  {
    std::cout << "\ndaily_repetition - end_never" << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::daily_repetition(), unix::end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition at 3 days - end_never" << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::daily_repetition(3), unix::end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition - end_after 12 " << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::daily_repetition(), unix::end_after(12));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix::timestamp _when;
    _when += _ent::days(4);
    std::cout << "\ndaily_repetition - end_on " << _when << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::daily_repetition(), unix::end_on(_when));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_never" << std::endl;

    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::weekly_repetition({_ent::weekday::mon, _ent::weekday::wed}),
        unix::end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed, at 2 weeks - "
                 "end_never"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::weekly_repetition({_ent::weekday::mon, _ent::weekday::wed}, 2),
        unix::end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition - end_never, with two weekdays"
              << std::endl
              << "If the weekday of today is *not* saturday, then the first "
                 "weekday is the weekday of today, and the second is saturday."
              << std::endl
              << "If the weekday of today is saturday, then the first weekday "
                 "is saturday, and the second one is wednesday."
              << std::endl;
    unix::timestamp _timestamp;
    _ent::weekday _weekday_one = _timestamp.get_weekday();
    _ent::weekday _weekday_two = _ent::weekday::sat;
    if (_weekday_one == _weekday_two) {
      _weekday_two = _ent::weekday::wed;
    }
    timestamps _sequence = unix::create_sequence(
        _timestamp, unix::weekly_repetition({_weekday_one, _weekday_two}),
        unix::end_never());
    for (unix::timestamp _aux : _sequence) {
      std::cout << _aux << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_after 12"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::weekly_repetition({_ent::weekday::mon, _ent::weekday::wed}),
        unix::end_after(12));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix::timestamp _when = unix::timestamp() + _ent::days(25);
    std::cout << "\nweekly_repetition on mon and wed - end_on " << _when
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::weekly_repetition({_ent::weekday::mon, _ent::weekday::wed}),
        unix::end_on(_when));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day - end_never" << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::monthly_repetition_by_day(_ent::day::d25),
        unix::end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 4 - end_after 25"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::monthly_repetition_by_day(_ent::day::d25, 4),
        unix::end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 24 - end_after 25"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::monthly_repetition_by_day(_ent::day::d25, 24),
        unix::end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 21 - end_after 25"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(), unix::monthly_repetition_by_day(_ent::day::d25, 21),
        unix::end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_week, use weekday two days "
                 "ahead of now, third of the month - end_after 4"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp() + _ent::days(2),
        unix::monthly_repetition_by_week(3, _ent::weekday::thu),
        unix::end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_week, use weekday two days "
                 "ahead of now, third of the month, at each 3 - end_after 4"
              << std::endl;
    timestamps _sequence = unix::create_sequence(
        unix::timestamp() + _ent::days(2),
        unix::monthly_repetition_by_week(3, _ent::weekday::thu, 4),
        unix::end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
        << "\nyearly_repetition, current month, two days ahead, end_after 4"
        << std::endl;
    unix::timestamp _now;
    _now += _ent::days(2);
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::yearly_repetition(_now.get_day(), _now.get_month()),
        unix::end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nyearly_repetition, current month, two days ahead, at each "
                 "5 - end_after 4"
              << std::endl;
    unix::timestamp _now;
    _now += _ent::days(2);
    timestamps _sequence = unix::create_sequence(
        unix::timestamp(),
        unix::yearly_repetition(_now.get_day(), _now.get_month(), 5),
        unix::end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  return 0;
}
