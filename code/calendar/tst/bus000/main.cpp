
#include <cstdint>
#include <iostream>
#include <vector>

//
#include <calendar/bus/create_sequence.h>
#include <calendar/bus/daily_repetition.h>
#include <calendar/bus/monthly_repetition.h>
#include <calendar/bus/weekly_repetition.h>
#include <calendar/bus/yearly_repetition.h>
#include <calendar/bus/ending_after.h>
#include <calendar/bus/ending_never.h>
#include <calendar/bus/ending_on.h>
#include <calendar/ent/second.h>
#include <calendar/ent/month.h>
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

int
main()
{


  typedef std::vector<unix::timestamp> timestamps;
  typedef bus::daily_repetition_t<unix::timestamp> daily_repetition;
  typedef bus::end_never_t<unix::timestamp> end_never;
  typedef bus::end_after_t<unix::timestamp>  end_after;
  typedef bus::end_on_t<unix::timestamp> end_on;
  typedef bus::weekly_repetition_t<unix::timestamp> weekly_repetition;
  typedef bus::monthly_repetition_by_day_t<unix::timestamp> monthly_repetition_by_day;
  typedef bus::monthly_repetition_by_week_t<unix::timestamp> monthly_repetition_by_week;
  typedef bus::yearly_repetition_t<unix::timestamp> yearly_repetition;

  {
    std::cout << "\ndaily_repetition - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), daily_repetition(), end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition at 3 days - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), daily_repetition(3), end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition - end_after 12 " << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), daily_repetition(), end_after(12));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix::timestamp _when;
    _when += ent::amount<tenacitas::calendar::ent::day>(4);
    std::cout << "\ndaily_repetition - end_on " << _when << ": " << _when;
    timestamps _sequence = create_sequence(
          unix::timestamp(), daily_repetition(), end_on(_when));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }


  {
    std::cout << "\nweekly_repetition on mon and wed - end_never" << std::endl;

    timestamps _sequence =
        create_sequence(unix::timestamp(),
                        weekly_repetition({ ent::weekday::mon, ent::weekday::wed }),
                        end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed, at 2 weeks - "
                 "end_never"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(),
          weekly_repetition({ ent::weekday::mon, ent::weekday::wed }, 2),
          end_never());
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
    ent::weekday _weekday_one = _timestamp.get_weekday();
    ent::weekday _weekday_two = ent::weekday::sat;
    if (_weekday_one == _weekday_two) {
      _weekday_two = ent::weekday::wed;
    }
    timestamps _sequence =
        create_sequence(_timestamp,
                        weekly_repetition({ _weekday_one, _weekday_two }),
                        end_never());
    for (unix::timestamp _aux : _sequence) {
      std::cout << _aux << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_after 12"
              << std::endl;
    timestamps _sequence =
        create_sequence(unix::timestamp(),
                        weekly_repetition({ ent::weekday::mon, ent::weekday::wed }),
                        end_after(12));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix::timestamp _when = unix::timestamp() + ent::amount<ent::day>(25);
    std::cout << "\nweekly_repetition on mon and wed - end_on " << _when
              << std::endl;
    timestamps _sequence =
        create_sequence(unix::timestamp(),
                        weekly_repetition({ ent::weekday::mon, ent::weekday::wed }),
                        end_on(_when));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }


  {
    std::cout << "\nmonthly_repetition_by_day - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), monthly_repetition_by_day(ent::day::d25), end_never());
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 4 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), monthly_repetition_by_day(ent::day::d25, 4), end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 24 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), monthly_repetition_by_day(ent::day::d25, 24), end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 21 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix::timestamp(), monthly_repetition_by_day(ent::day::d25, 21), end_after(25));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
        << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
           "now, third of the month - end_after 4"
        << std::endl;
    timestamps _sequence =
        create_sequence(unix::timestamp() + ent::amount<ent::day>(2),
                        monthly_repetition_by_week(3, ent::weekday::thu),
                        end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
        << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
           "now, third of the month, at each 3 - end_after 4"
        << std::endl;
    timestamps _sequence =
        create_sequence(unix::timestamp() + ent::amount<ent::day>(2),
                        monthly_repetition_by_week(3, ent::weekday::thu, 4),
                        end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nyearly_repetition, current month, two days ahead, "
                 "end_after 4"
              << std::endl;
    unix::timestamp _now;
    _now += ent::amount<ent::day>(2);
    timestamps _sequence =
        create_sequence(unix::timestamp(),
                        yearly_repetition(_now.get_day(), _now.get_month()),
                        end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nyearly_repetition, current month, two days ahead, at each 5 - end_after 4"
              << std::endl;
    unix::timestamp _now;
    _now += ent::amount<ent::day>(2);
    timestamps _sequence =
        create_sequence(unix::timestamp(),
                        yearly_repetition(_now.get_day(), _now.get_month(), 5),
                        end_after(4));
    for (unix::timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  return 0;
}
