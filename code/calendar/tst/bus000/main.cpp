
#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

//
#include <calendar/ent/ending_after.h>
#include <calendar/ent/ending_never.h>
#include <calendar/ent/ending_on.h>
#include <calendar/ent/second.h>
#include <calendar/ent/timestamp_second.h>

using namespace tenacitas::calendar::ent;

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

template<typename t_time_precision>
struct daily_repetition
{
  typedef timestamp_t<t_time_precision> timestamp;

  daily_repetition(uint16_t p_at_each = 1)
    : m_at_each(p_at_each)
  {}

  timestamp prev(timestamp p_time) { return p_time - m_at_each; }

  timestamp next(timestamp p_time, bool p_first = false)
  {
    return (p_first ? p_time : p_time + m_at_each);
  }

private:
  amount<day> m_at_each;
};

// struct weekly_repetition
//{

//  weekly_repetition(std::vector<weekday>&& p_weekdays, uint16_t p_at_each = 1)
//    : m_weekdays(std::move(p_weekdays))
//    , m_at_each(p_at_each)
//  {}

//  times next(time_t p_time, bool p_first = false)
//  {
//    times _sequence;
//    if (p_first) {
//      first(p_time, _sequence);
//      return _sequence;
//    }

//    time_t _time = begin_week(p_time);
//    //
//    rest(m_weekdays.begin(), _time, _sequence);
//    return _sequence;
//  }

// private:
//  time_t begin_week(time_t p_time)
//  {
//    time_t _time = p_time + (m_at_each * week_in_seconds);
//    struct tm* _tm = localtime(&p_time);
//    _time -= static_cast<uint16_t>(_tm->tm_wday) * day_in_seconds;
//    return _time;
//  }

//  time_t next_timestamp_in_weekday_from_timestamp(time_t p_time,
//                                                  weekday p_weekday)
//  {
//    struct tm* _tm = localtime(&p_time);
//    weekday _weekday = static_cast<weekday>(_tm->tm_wday);

//    time_t _time = p_time;

//    if (_weekday == p_weekday) {
//      return _time;
//    }

//    if (_weekday < p_weekday) {
//      uint16_t _diff =
//        static_cast<uint16_t>(p_weekday) - static_cast<uint16_t>(_weekday);
//      _time += _diff * day_in_seconds;
//    } else {
//      _time += (static_cast<uint16_t>(weekday::sat) -
//                static_cast<uint16_t>(_weekday)) *
//               day_in_seconds;
//      _time += day_in_seconds;
//      _time += (static_cast<uint16_t>(p_weekday) -
//                static_cast<uint16_t>(weekday::sun)) *
//               day_in_seconds;
//    }
//    return _time;
//  }

//  void rest(std::vector<weekday>::const_iterator p_weekday,
//            time_t p_time,
//            times& p_sequence)
//  {
//    time_t _time = p_time;
//    for (std::vector<weekday>::const_iterator _ite = p_weekday;
//         _ite != m_weekdays.end();
//         ++_ite) {
//      struct tm* _tm = localtime(&_time);
//      weekday _weekday = static_cast<weekday>(_tm->tm_wday);
//      _time +=
//        (static_cast<uint16_t>(*_ite) - static_cast<uint16_t>(_weekday)) *
//        day_in_seconds;
//      p_sequence.push_back(_time);
//    }
//  }

//  void first(time_t p_time, times& p_sequence)
//  {
//    time_t _time =
//      next_timestamp_in_weekday_from_timestamp(p_time, m_weekdays[0]);
//    p_sequence.push_back(_time);
//    rest(++m_weekdays.begin(), _time, p_sequence);
//  }

// private:
//  std::vector<weekday> m_weekdays;
//  uint16_t m_at_each;
//};

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

template<typename t_time_precision,
         typename t_repetition,
         typename t_ending_type>
std::vector<timestamp_t<t_time_precision>>
create_sequence(timestamp_t<t_time_precision> p_start,
                t_repetition&& p_repetition,
                t_ending_type&& p_end)
{
  std::vector<timestamp_t<t_time_precision>> _sequence;

  timestamp_t<t_time_precision> _time = p_repetition.next(p_start, true);
  while (!p_end.stop(_time)) {
    _sequence.push_back(_time);
    _time = p_repetition.next(_time);
  }
  return _sequence;
}

// template<typename t_time_precision, typename t_ending_type>
// std::vector<timestamp<t_time_precision>>
// create_sequence(timestamp<t_time_precision> p_start,
//                weekly_repetition&& p_repetition,
//                t_ending_type&& p_end)
//{
//  times _sequence;

//  times _times = p_repetition.next(p_start, true);
//  while (!p_end.stop(_times.back())) {
//    _sequence.insert(_sequence.end(), _times.begin(), _times.end());
//    _times = p_repetition.next(_times.back() + day_in_seconds);
//  }
//  return _sequence;
//}

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
  //  {
  //    std::cout << "\nweekly_repetition on mon and wed - end_never" <<
  //    std::endl; times _sequence =
  //      create_sequence(timestamp<second>(),
  //                      weekly_repetition({ weekday::mon, weekday::wed }),
  //                      end_never<second>());
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    std::cout << "\nweekly_repetition on mon and wed, at 2 weeks - "
  //                 "end_never"
  //              << std::endl;
  //    times _sequence =
  //      create_sequence(timestamp<second>(),
  //                      weekly_repetition({ weekday::mon, weekday::wed }, 2),
  //                      end_never<second>());
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

  //  {
  //    std::cout << "\nweekly_repetition - end_never, with "
  //                 "first weekday of the "
  //                 "first timestamp in the weekdays of the repetition"
  //              << std::endl;
  //    time_t _time = time(nullptr);
  //    struct tm* _tm = localtime(&_time);
  //    weekday _weekday_one = static_cast<weekday>(_tm->tm_wday);
  //    weekday _weekday_two = weekday::sat;
  //    if (_weekday_one == _weekday_two) {
  //      _weekday_two = weekday::wed;
  //    }
  //    times _sequence =
  //      create_sequence(time(nullptr),
  //                      weekly_repetition({ _weekday_one, _weekday_two }),
  //                      end_never());
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }

  //  {
  //    std::cout << "\nweekly_repetition on mon and wed - end_after 12"
  //              << std::endl;
  //    times _sequence =
  //      create_sequence(time(nullptr),
  //                      weekly_repetition({ weekday::mon, weekday::wed }),
  //                      end_after(12));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
  //  {
  //    time_t _when = time(nullptr) + 25 * day_in_seconds;
  //    std::cout << "\nweekly_repetition on mon and wed - end_on " << _when <<
  //    ": "
  //              << ctime(&_when);
  //    times _sequence =
  //      create_sequence(time(nullptr),
  //                      weekly_repetition({ weekday::mon, weekday::wed }),
  //                      end_on(_when));
  //    for (time_t _time : _sequence) {
  //      std::cout << ctime(&_time);
  //    }
  //  }
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
