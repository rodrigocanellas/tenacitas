
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
#include <calendar/ent/unix.h>

using namespace tenacitas::calendar::ent;
using namespace tenacitas::calendar::bus;
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


  typedef std::vector<unix> timestamps;
  typedef daily_repetition_t<unix> daily_repetition;
  typedef end_never_t<unix> end_never;
  typedef end_after_t<unix>  end_after;
  typedef end_on_t<unix> end_on;
  typedef weekly_repetition_t<unix> weekly_repetition;
  typedef monthly_repetition_by_day_t<unix> monthly_repetition_by_day;
  typedef monthly_repetition_by_week_t<unix> monthly_repetition_by_week;
  typedef yearly_repetition_t<unix> yearly_repetition;

  {
    std::cout << "\ndaily_repetition - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix(), daily_repetition(), end_never());
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition at 3 days - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix(), daily_repetition(3), end_never());
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition - end_after 12 " << std::endl;
    timestamps _sequence = create_sequence(
          unix(), daily_repetition(), end_after(12));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix _when;
    _when += amount<tenacitas::calendar::ent::day>(4);
    std::cout << "\ndaily_repetition - end_on " << _when << ": " << _when;
    timestamps _sequence = create_sequence(
          unix(), daily_repetition(), end_on(_when));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }


  {
    std::cout << "\nweekly_repetition on mon and wed - end_never" << std::endl;

    timestamps _sequence =
        create_sequence(unix(),
                        weekly_repetition({ weekday::mon, weekday::wed }),
                        end_never());
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed, at 2 weeks - "
                 "end_never"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix(),
          weekly_repetition({ weekday::mon, weekday::wed }, 2),
          end_never());
    for (unix _timestamp : _sequence) {
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
    unix _timestamp;
    weekday _weekday_one = _timestamp.get_weekday();
    weekday _weekday_two = weekday::sat;
    if (_weekday_one == _weekday_two) {
      _weekday_two = weekday::wed;
    }
    timestamps _sequence =
        create_sequence(_timestamp,
                        weekly_repetition({ _weekday_one, _weekday_two }),
                        end_never());
    for (unix _aux : _sequence) {
      std::cout << _aux << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_after 12"
              << std::endl;
    timestamps _sequence =
        create_sequence(unix(),
                        weekly_repetition({ weekday::mon, weekday::wed }),
                        end_after(12));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    unix _when = unix() + amount<day>(25);
    std::cout << "\nweekly_repetition on mon and wed - end_on " << _when
              << std::endl;
    timestamps _sequence =
        create_sequence(unix(),
                        weekly_repetition({ weekday::mon, weekday::wed }),
                        end_on(_when));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }


  {
    std::cout << "\nmonthly_repetition_by_day - end_never" << std::endl;
    timestamps _sequence = create_sequence(
          unix(), monthly_repetition_by_day(day::d25), end_never());
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 4 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix(), monthly_repetition_by_day(day::d25, 4), end_after(25));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 24 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix(), monthly_repetition_by_day(day::d25, 24), end_after(25));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nmonthly_repetition_by_day, at each 21 - "
                 "end_after 25"
              << std::endl;
    timestamps _sequence = create_sequence(
          unix(), monthly_repetition_by_day(day::d25, 21), end_after(25));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
        << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
           "now, third of the month - end_after 4"
        << std::endl;
    timestamps _sequence =
        create_sequence(unix() + amount<day>(2),
                        monthly_repetition_by_week(3, weekday::thu),
                        end_after(4));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
        << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
           "now, third of the month, at each 3 - end_after 4"
        << std::endl;
    timestamps _sequence =
        create_sequence(unix() + amount<day>(2),
                        monthly_repetition_by_week(3, weekday::thu, 4),
                        end_after(4));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nyearly_repetition, current month, two days ahead, "
                 "end_after 4"
              << std::endl;
    unix _now;
    _now += amount<day>(2);
    timestamps _sequence =
        create_sequence(unix(),
                        yearly_repetition(_now.get_day(), _now.get_month()),
                        end_after(4));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nyearly_repetition, current month, two days ahead, at each 5 - end_after 4"
              << std::endl;
    unix _now;
    _now += amount<day>(2);
    timestamps _sequence =
        create_sequence(unix(),
                        yearly_repetition(_now.get_day(), _now.get_month(), 5),
                        end_after(4));
    for (unix _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  return 0;
}
