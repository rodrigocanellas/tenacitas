
#include <cstdint>
#include <iostream>
#include <vector>

//
#include <calendar/bus/create_sequence.h>
#include <calendar/bus/daily_repetition.h>
#include <calendar/bus/monthly_repetition.h>
#include <calendar/bus/weekly_repetition.h>
#include <calendar/ent/ending_after.h>
#include <calendar/ent/ending_never.h>
#include <calendar/ent/ending_on.h>
#include <calendar/ent/second.h>
#include <calendar/ent/month.h>
#include <calendar/ent/timestamp_second.h>

using namespace tenacitas::calendar::ent;
using namespace tenacitas::calendar::bus;

template <typename t_time_precision>
 struct monthly_repetition_by_day
{
   typedef timestamp_t<t_time_precision> timestamp;

  monthly_repetition_by_day(day p_day, uint16_t p_at_each = 1)
    : m_day(p_day)
    , m_at_each(p_at_each)
  {
  }

  timestamp next(timestamp p_time, bool p_first = false)
  {

    timestamp _time = p_time;

    day _day(m_day);

    amount<day> _days (month::days(_time.get_month(), _time.get_year()));

    if (amount<day>(_day) > _days) {
      _day = day::create(_days);
    }

    _time.set_day(_day);

    if (!p_first) {
      _time += amount<month>(m_at_each);
    }
    return _time;
  }

 private:
  day m_day;
  amount<month> m_at_each;
};

// struct yearly_repetition
//{
//  yearly_repetition(day p_day, month p_month, uint16_t p_at_each = 1)
//    : m_day(p_day)
//    , m_month(p_month)
//    , m_at_each(p_at_each)
//  {}

//  time_t next(time_t p_time, bool p_first = false)
//  {
//    struct tm* _tm = localtime(&p_time);
//    decltype(_tm->tm_mon) _month =
//    static_cast<decltype(_tm->tm_mon)>(m_month); decltype(_tm->tm_mday) _day =
//    static_cast<decltype(_tm->tm_mday)>(m_day); if (_month > _tm->tm_mon) {
//      _tm->tm_mon = _month;
//      _tm->tm_mday = _day;
//    } else if (_month == _tm->tm_mon) {
//      if (_day > _tm->tm_mday) {
//        _tm->tm_mday = _day;
//      } else if (_day <= _tm->tm_mday) {
//        // _day < _tm->tm_mday
//        ++_tm->tm_mon;
//        if (_tm->tm_mon > 11) {
//          _tm->tm_mon = 0;
//          //          ++_tm->tm_year;
//        }
//        _tm->tm_mday = _day;
//      }
//    } else {
//      // _month < _tm->tm_mon
//      //      ++_tm->tm_year;
//      _tm->tm_mon = _month;
//      _tm->tm_mday = _day;
//    }

//    if (!p_first) {
//      _tm->tm_year += m_at_each;
//      //      _tm->tm_mon += m_at_each % 12;
//    }
//    return mktime(_tm);
//  }

// private:
//  day m_day;
//  month m_month;
//  uint16_t m_at_each;
//};

int
main()
{

  typedef timestamp_t<second> timestamp;
  typedef std::vector<timestamp> timestamps;

  {
    std::cout << "\ndaily_repetition - end_never" << std::endl;
    timestamps _sequence = create_sequence(
      timestamp(), daily_repetition<second>(), end_never<second>());
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition at 3 days - end_never" << std::endl;
    timestamps _sequence = create_sequence(
      timestamp(), daily_repetition<second>(3), end_never<second>());
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\ndaily_repetition - end_after 12 " << std::endl;
    timestamps _sequence = create_sequence(
      timestamp(), daily_repetition<second>(), end_after<second>(12));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    timestamp _when;
    _when += amount<tenacitas::calendar::ent::day>(4);
    std::cout << "\ndaily_repetition - end_on " << _when << ": " << _when;
    timestamps _sequence = create_sequence(
      timestamp(), daily_repetition<second>(), end_on<second>(_when));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_never" << std::endl;

    timestamps _sequence =
      create_sequence(timestamp(),
                      weekly_repetition<second>({ weekday::mon, weekday::wed }),
                      end_never<second>());
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed, at 2 weeks - "
                 "end_never"
              << std::endl;
    timestamps _sequence = create_sequence(
      timestamp(),
      weekly_repetition<second>({ weekday::mon, weekday::wed }, 2),
      end_never<second>());
    for (timestamp _timestamp : _sequence) {
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
    timestamp _timestamp;
    weekday _weekday_one = _timestamp.get_weekday();
    weekday _weekday_two = weekday::sat;
    if (_weekday_one == _weekday_two) {
      _weekday_two = weekday::wed;
    }
    timestamps _sequence =
      create_sequence(_timestamp,
                      weekly_repetition<second>({ _weekday_one, _weekday_two }),
                      end_never<second>());
    for (timestamp _aux : _sequence) {
      std::cout << _aux << std::endl;
    }
  }

  {
    std::cout << "\nweekly_repetition on mon and wed - end_after 12"
              << std::endl;
    timestamps _sequence =
      create_sequence(timestamp(),
                      weekly_repetition<second>({ weekday::mon, weekday::wed }),
                      end_after<second>(12));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    timestamp _when = timestamp() + amount<day>(25);
    std::cout << "\nweekly_repetition on mon and wed - end_on " << _when
              << std::endl;
    timestamps _sequence =
      create_sequence(timestamp(),
                      weekly_repetition<second>({ weekday::mon, weekday::wed }),
                      end_on<second>(_when));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  //  {
  //    std::cout << "\nmonthly_repetition_by_day - end_never" << std::endl;
  //    times _sequence = create_sequence(
  //      time(nullptr), monthly_repetition_by_day(25), end_never());
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    std::cout << "\nmonthly_repetition_by_day, at each 4 - "
  //                 "end_after 25"
  //              << std::endl;
  //    times _sequence = create_sequence(
  //      time(nullptr), monthly_repetition_by_day(25, 4), end_after(25));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

  //  {
  //    std::cout << "\nmonthly_repetition_by_day, at each 24 - "
  //                 "end_after 25"
  //              << std::endl;
  //    times _sequence = create_sequence(
  //      time(nullptr), monthly_repetition_by_day(25, 24), end_after(25));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    std::cout << "\nmonthly_repetition_by_day, at each 21 - "
  //                 "end_after 25"
  //              << std::endl;
  //    times _sequence = create_sequence(
  //      time(nullptr), monthly_repetition_by_day(25, 21), end_after(25));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

  {
    std::cout
      << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
         "now, third of the month - end_after 4"
      << std::endl;
    timestamps _sequence =
      create_sequence(timestamp() + amount<day>(2),
                      monthly_repetition_by_week<second>(3, weekday::thu),
                      end_after<second>(4));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  {
    std::cout
      << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
         "now, third of the month, at each 3 - end_after 4"
      << std::endl;
    timestamps _sequence =
      create_sequence(timestamp() + amount<day>(2),
                      monthly_repetition_by_week<second>(3, weekday::thu, 4),
                      end_after<second>(4));
    for (timestamp _timestamp : _sequence) {
      std::cout << _timestamp << std::endl;
    }
  }

  //  {
  //    std::cout << "\nyearly_repetition, current month, two days ahead, "
  //                 "end_after 4"
  //              << std::endl;
  //    time_t _now = time(nullptr);
  //    _now += 2 * day_in_seconds;
  //    struct tm* _tm = localtime(&_now);
  //    times _sequence =
  //      create_sequence(time(nullptr),
  //                      yearly_repetition(static_cast<day>(_tm->tm_mday),
  //                                        static_cast<month>(_tm->tm_mon)),
  //                      end_after(4));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    std::cout << "\nyearly_repetition, current month, two days ahead, at
  //    each "
  //                 "5 - end_after 4"
  //              << std::endl;
  //    time_t _now = time(nullptr);
  //    _now += 2 * day_in_seconds;
  //    struct tm* _tm = localtime(&_now);
  //    times _sequence = create_sequence(
  //      time(nullptr),
  //      yearly_repetition(
  //        static_cast<day>(_tm->tm_mday), static_cast<month>(_tm->tm_mon), 5),
  //      end_after(4));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

  return 0;
}
