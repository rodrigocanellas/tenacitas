
#include <cstdint>
#include <iostream>
#include <vector>

//
#include <calendar/bus/create_sequence.h>
#include <calendar/bus/daily_repetition.h>
#include <calendar/bus/weekly_repetition.h>
#include <calendar/ent/ending_after.h>
#include <calendar/ent/ending_never.h>
#include <calendar/ent/ending_on.h>
#include <calendar/ent/second.h>
#include <calendar/ent/timestamp_second.h>

using namespace tenacitas::calendar::ent;
using namespace tenacitas::calendar::bus;

// enum class weekday : uint8_t
//{
//  sun = 0,
//  mon = 1,
//  tue = 2,
//  wed = 3,
//  thu = 4,
//  fri = 5,
//  sat = 6
//};

// typedef uint8_t day;

// typedef uint16_t year;

// enum class month : uint8_t
//{
//  jan = 0,
//  feb = 1,
//  mar = 2,
//  apr = 3,
//  may = 4,
//  jun = 5,
//  jul = 6,
//  ago = 7,
//  sep = 8,
//  oct = 9,
//  nov = 10,
//  dec = 11
//};

// typedef std::vector<time_t> times;

// static const uint8_t days_in_month[static_cast<uint8_t>(month::dec) + 1] = {
//  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
//};

// static const uint8_t seconds = 1;
// static const uint16_t minute_in_seconds = 60 * seconds;
// static const uint16_t hour_in_seconds = 60 * minute_in_seconds;
// static const uint32_t day_in_seconds = 24 * hour_in_seconds;
// static const uint32_t week_in_seconds = 7 * day_in_seconds;

//                     repetition

//// incomplete
// struct monthly_repetition_by_week
//{

//  monthly_repetition_by_week(uint8_t p_order,
//                             weekday p_weekday,
//                             uint16_t p_at_each = 1)
//    : m_order(p_order)
//    , m_weekday(p_weekday)
//    , m_at_each(p_at_each)
//  {}

//  time_t next(time_t p_time, bool p_first = false)
//  {

//    struct tm* _tm = localtime(&p_time);

//    if (!p_first) {
//      _tm->tm_year += m_at_each / 12;
//      _tm->tm_mon += m_at_each % 12;
//      time_t _aux = mktime(_tm);
//      _tm = localtime(&_aux);
//    }

//    _tm->tm_mday = 1;
//    time_t _first_day_of_month = mktime(_tm);
//    _tm = localtime(&_first_day_of_month);

//    int _weekday_first_day = _tm->tm_wday;
//    int _weekday_target = static_cast<int>(m_weekday);
//    int _diff = -1;
//    if (_weekday_target == _weekday_first_day) {
//      _diff = 0;
//    } else if (_weekday_target > _weekday_first_day) {
//      _diff = _weekday_target - _weekday_first_day;
//    } else {
//      _diff = static_cast<int>(weekday::sat) - _weekday_first_day +
//              _weekday_target + 1;
//    }

//    time_t _first_day_with_weekday_same_as_target =
//      _first_day_of_month +
//      (static_cast<decltype(_first_day_of_month)>(_diff) * day_in_seconds);
//    _tm = localtime(&_first_day_with_weekday_same_as_target);

//    time_t _target =
//      _first_day_with_weekday_same_as_target + (m_order - 1) *
//      week_in_seconds;

//    return _target;
//  }

// private:
//  uint8_t m_order;
//  weekday m_weekday;
//  uint16_t m_at_each;
//};

// struct monthly_repetition_by_day
//{
//  monthly_repetition_by_day(day p_day, uint16_t p_at_each = 1)
//    : m_day(p_day)
//    , m_at_each(p_at_each)
//  {
//    if (m_day > 31) {
//      throw new std::runtime_error(std::to_string(m_day) +
//                                   " is not a valid day");
//    }
//  }

//  time_t next(time_t p_time, bool p_first = false)
//  {

//    time_t _time = p_time;
//    struct tm* _tm = localtime(&_time);

//    day _day = m_day;
//    if ((_tm->tm_mon != 1)) {
//      if (_day > days_in_month[_tm->tm_mon]) {
//        _day = days_in_month[_tm->tm_mon];
//      }
//    } else {
//      // february
//      if ((_tm->tm_year % 4) == 0) {
//        if (_day > 29) {
//          _day = 29;
//        } else if (_day > 28) {
//          _day = 28;
//        }
//      }
//    }

//    _tm->tm_mday = _day;

//    if (!p_first) {
//      _tm->tm_year += m_at_each / 12;
//      _tm->tm_mon += m_at_each % 12;
//    }
//    return mktime(_tm);
//  }

// private:
//  day m_day;
//  uint16_t m_at_each;
//};

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

  //  {
  //    std::cout
  //      << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
  //         "now, third of the month - end_after 4"
  //      << std::endl;
  //    times _sequence =
  //      create_sequence(time(nullptr) + 2 * day_in_seconds,
  //                      monthly_repetition_by_week(3, weekday::thu),
  //                      end_after(4));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    std::cout
  //      << "\nmonthly_repetition_by_week, calcuate weekday two days ahead "
  //         "now, third of the month, at each 3 - end_after 4"
  //      << std::endl;
  //    times _sequence =
  //      create_sequence(time(nullptr) + 2 * day_in_seconds,
  //                      monthly_repetition_by_week(3, weekday::thu, 4),
  //                      end_after(4));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

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
