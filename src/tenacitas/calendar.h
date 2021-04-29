#ifndef TENACITAS_CALENDAR_H
#define TENACITAS_CALENDAR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

/// \brief master namespace
namespace tenacitas {

/// \brief about calendar and time
namespace calendar {

/// \brief Converts time formats
///
/// \tparam t_to destination type
///
/// \tparam t_from origin type
///
/// \param p_from origin value
///
/// \return p_from converted to t_to type
// template <typename t_to, typename t_from> inline t_to convert(t_from p_from)
// {
//  return std::chrono::duration_cast<t_to>(p_from);
//}
template <typename t_to, typename t_from>
inline t_to convert(const t_from &p_from) {
  return std::chrono::duration_cast<t_to>(p_from);
}

/// \brief Calculates 'now' in different time units
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true> struct now {

  /// \brief 'now' in microseconds
  template <typename t_int = uint64_t> inline static t_int microsecs() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief 'now' as a string in the sformat YYYY-MM-DD HH::MM::SS,999999
  static std::string iso8601_microsecs() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = microsecs();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000000);

    const auto _remainder = _microsecs % 1000000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(6) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief A specific time as a string in the format 'YYYY-MM-DD
  /// HH::MM::SS,999999'
  ///
  /// \param p_microsecs is the time one wants to convert to string
  static std::string iso8601_microsecs(uint64_t p_microsecs) {
    using namespace std;
    using namespace chrono;

    const time_t _time_t = static_cast<time_t>(p_microsecs / 1000000);

    const auto _remainder = p_microsecs % 1000000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(6) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief 'now' in milliseconds
  template <typename t_int = uint64_t> inline static t_int millisecs() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief 'now' as a string in the format 'YYYY-MM-DD HH::MM::SS,999'
  // TODO test iso8601_millisecs
  static std::string iso8601_millisecs() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = millisecs();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000);

    const auto _remainder = _microsecs % 1000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(3) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief 'now' in seconds
  template <typename t_int = uint64_t> inline static t_int secs() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief 'now' as a string in the format 'YYYY-MM-DD HH::MM::SS'
  // TODO test iso8601_secs
  static std::string iso8601_secs() {
    using namespace std;
    using namespace chrono;

    const time_t _time_t = secs<time_t>();

    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S");
    return _stream.str();
  }

  /// \brief 'now' in minutes
  // TODO test minutes
  template <typename t_int = uint64_t> inline static t_int minutes() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief 'now' in hours
  // TODO test hours
  template <typename t_int = uint64_t> inline static t_int hours() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief 'now' in days
  // TODO test days
  template <typename t_int = uint64_t> inline static t_int days() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count() /
        24);
  }

  /// \brief 'now' in months
  // TODO test months
  template <typename t_int = uint64_t> inline static t_int months() {
    return static_cast<t_int>(days() / 30);
  }

  /// \brief 'now' in years
  // TODO test years
  template <typename t_int = uint64_t> inline static t_int years() {
    return static_cast<t_int>(months() / 12);
  }
};
} // namespace calendar

} // namespace tenacitas

#endif

#if 0

#ifndef TENACITAS_CALENDAR_GREGORIAN_ADD_H
#define TENACITAS_CALENDAR_GREGORIAN_ADD_H

#include <calendar/gregorian/days.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

template <typename t_timestamp>
t_timestamp add_days(t_timestamp p_timestamp, days p_days) {
  days _days = p_timestamp;
  _days += p_days;
  return t_timestamp(_days);
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // ADD_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_CREATE_SEQUENCE_H
#define TENACITAS_CALENDAR_GREGORIAN_CREATE_SEQUENCE_H

#include <vector>

#include <calendar/_bus/weekly_repetition.h>
#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

///
/// \brief creates a sequence o \p timestamp objects based on a initial \p
/// timestamp, on a repetition pattern, an on an ending condition
///
/// \tparam t_timestamp
///
/// \tparam t_repetition defines the repetition pattern. The class must define
///
/// \code timestamp next(timestamp p_time, bool p_first = false) \endcode
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement
///
/// \code bool stop(timestamp p_time) \endcode.
///
template <typename t_timestamp, typename t_repetition, typename t_ending_type>
std::vector<t_timestamp> create_sequence(t_timestamp p_start,
                                         t_repetition &&p_repetition,
                                         t_ending_type &&p_end) {
  std::vector<t_timestamp> _sequence;

  t_timestamp _time = p_repetition.next(p_start, true);
  while (!p_end.stop(_time)) {
    _sequence.push_back(_time);
    _time = p_repetition.next(_time);
  }
  return _sequence;
}

///
/// \brief creates a sequence o \p timestamp objects based on a initial \p
/// timestamp, on a \p weekly_repetition pattern, an on an ending condition
///
/// \tparam t_timestamp
///
/// \tparam t_ending_type defines when the sequence creation must stop. It must
/// implement \code bool stop(timestamp p_time) \endcode.
///
template <typename t_timestamp, typename t_ending_type>
std::vector<t_timestamp>
create_sequence(t_timestamp p_start,
                weekly_repetition_t<t_timestamp> &&p_repetition,
                t_ending_type &&p_end)

{
  typedef t_timestamp timestamp;
  typedef std::vector<timestamp> timestamps;

  timestamps _sequence;

  timestamps _times = p_repetition.next(p_start, true);
  while (!p_end.stop(_times.back())) {
    _sequence.insert(_sequence.end(), _times.begin(), _times.end());
    timestamp _next = _times.back() /*+ _ent::days(1)*/;
    _times = p_repetition.next(_next);
  }
  return _sequence;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // CREATE_SEQUENCE_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_DAILY_REPETITION_T_H
#define TENACITAS_CALENDAR_GREGORIAN_DAILY_REPETITION_T_H

#include <cstdint>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

///
/// \brief Calculates the next and previous timestamp, given an amount of days
///
template <typename t_timestamp> struct daily_repetition_t {
  ///
  /// \brief daily_repetition_t
  /// \param p_at_each the amount of days between a timestamp and the next, or
  /// previous
  ///
  inline daily_repetition_t(uint16_t p_at_each = 1) : m_at_each(p_at_each) {}

  ///
  /// \brief prev calculates the previous timestamp to a given one
  ///
  /// \param p_time timestamp from each the previous timestamp should be
  /// calculated
  ///
  /// \param p_first if \p true, the timestamp returned is \p p_time
  /// itself; otherwise, it actually calculates the previous timstamp
  ///
  /// \return the previous timestamp to \p p_time, or \p p_time, if \p p_first
  /// is true
  ///
  inline t_timestamp prev(t_timestamp p_time, bool p_first = false) {
    return (p_first ? p_time : p_time - m_at_each);
  }

  ///
  /// \brief prev calculates the next timestamp to a given one
  ///
  /// \param p_time timestamp from each the next timestamp should be
  /// calculated
  ///
  /// \param p_first if \p true, the timestamp returned is \p p_time
  /// itself; otherwise, it actually calculates the next timestamp
  ///
  /// \return the next timestamp to \p p_time, or \p p_time, if \p p_first
  /// is true
  ///
  inline t_timestamp next(t_timestamp p_time, bool p_first = false) {
    return (p_first ? p_time : p_time + m_at_each);
  }

private:
  ///
  /// \brief m_at_each amount of days for the next, or previous, timestamp
  ///
  _ent::days m_at_each;
};

} // namespace greogorian
} // namespace calendar
} // namespace tenacitas

#endif // daily_repetition_t_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_ENDING_AFTER_H
#define TENACITAS_CALENDAR_GREGORIAN_ENDING_AFTER_H

#include <cstdint>
#include <ctime>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template<typename t_timestamp>
struct end_after_t
{
  end_after_t(uint16_t p_number)
    : m_number(p_number)
  {}

  bool stop(const t_timestamp &)
  {
    return (++m_counter > m_number ? true : false);
  }

  uint16_t m_number;
  uint16_t m_counter = 0;
};

} // namespace gregorian
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AFTER_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_ENDING_NEVER_H
#define TENACITAS_CALENDAR_GREGORIAN_ENDING_NEVER_H

#include <cstdint>
#include <ctime>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template <typename t_timestamp> struct end_never_t {
  end_never_t(uint32_t p_max_events = 40) : m_max_events(p_max_events) {}

  bool stop(const t_timestamp &) {
    if (m_num == m_max_events) {
      return true;
    }
    ++m_num;
    return false;
  }

private:
  uint32_t m_num = 0;
  uint32_t m_max_events;
};

} // namespace greogorian
} // namespace calendar
} // namespace tenacitas
#endif // ENDING_NEVER_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_ENDING_AT_H
#define TENACITAS_CALENDAR_GREGORIAN_ENDING_AT_H

#include <ctime>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template<typename t_timestamp>
struct end_on_t
{
  end_on_t(t_timestamp p_when)
    : m_when(p_when)
  {}

  bool stop(const t_timestamp & p_time)
  {
    return (p_time > m_when ? true : false);
  }

  t_timestamp m_when;
};

} // namespace gregorian
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AT_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_MONTHLY_REPETITION_H
#define TENACITAS_CALENDAR_GREGORIAN_MONTHLY_REPETITION_H

#include <cstdint>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/months.h>
#include <calendar/gregorian/weekday.h>
#include <calendar/gregorian/weekdays.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

///
/// \brief Calculates the next t_timestamp, given a weekday, the order of the
/// weekday in a month, and the interval between the months
///
/// \tparam needs to be copy constructable
///
template <typename t_timestamp> struct monthly_repetition_by_week_t {

  ///
  /// \brief monthly_repetition_by_week_t
  /// \param p_order
  /// \param p_weekday
  /// \param p_at_each
  ///
  monthly_repetition_by_week_t(uint8_t p_order, _ent::weekday p_weekday,
                               uint16_t p_at_each = 1)
      : m_order(p_order), m_weekday(p_weekday), m_at_each(p_at_each) {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  t_timestamp next(t_timestamp p_time, bool p_first = false) {
    t_timestamp _timestamp = p_time;
    if (!p_first) {
      _timestamp += m_at_each;
    }

    t_timestamp _day_one(_timestamp);
    _day_one.set_day(_ent::day::d01);

    t_timestamp _first_target_weekday(_day_one);
    _first_target_weekday += _ent::days(m_weekday - _day_one.get_weekday());

    t_timestamp _target(_first_target_weekday);

    _target += _ent::weekdays(m_order - 1);

    return _target;
  }

private:
  ///
  /// \brief m_order
  ///
  uint8_t m_order;

  ///
  /// \brief m_weekday
  ///
  _ent::weekday m_weekday;

  ///
  /// \brief m_at_each
  ///
  _ent::months m_at_each;
};

///
///
///
template <typename t_timestamp> struct monthly_repetition_by_day_t {
  monthly_repetition_by_day_t(_ent::day p_day, uint16_t p_at_each = 1)
      : m_day(p_day), m_at_each(p_at_each) {}

  t_timestamp next(t_timestamp p_time, bool p_first = false) {

    t_timestamp _time(p_time);

    _ent::day _day(m_day);

    _ent::days _days(_ent::month::get_days(_time.get_month(), _time.get_year()));

    if (_ent::days(_day) > _days) {
      _day = _ent::day::create(_days.get<uint8_t>());
    }

    _time.set_day(_day);

    if (!p_first) {
      _time += _ent::months(m_at_each);
    }
    return _time;
  }

private:
  _ent::day m_day;
  _ent::months m_at_each;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MONTHLY_REPETITION_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_TIMESTAMP_H
#define TENACITAS_CALENDAR_GREGORIAN_TIMESTAMP_H

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 04/2020
///
/// \attention This implementation is based on the the code published at
/// https://howardhinnant.github.io/date_algorithms.html. Thanks Howard Hinnant,
/// for your contribution.

#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <tuple>

#include <measures/time/amounts.h>
#include <measures/time/day.h>
#include <measures/time/hour.h>
#include <measures/time/minute.h>
#include <measures/time/month.h>
#include <measures/time/second.h>
#include <measures/time/weekday.h>
#include <measures/time/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The timestamp struct represents a certain moment in time, with
/// seconds precision
///
struct timestamp {
  ///
  /// \brief timestamp default constructor creates a timestamp for now
  ///
  timestamp() = default;

  ///
  /// \brief timestamp
  /// \param p_year
  /// \param p_month
  /// \param p_day
  /// \param p_hour
  /// \param p_minute
  /// \param p_second
  ///
  timestamp(measures::time::year p_year, measures::time::month p_month, measures::time::day p_day, measures::time::hour p_hour = measures::time::hour::_00,
            measures::time::minute p_minute = measures::time::minute::_00, measures::time::second p_second = measures::time::second::_00);
  ///
  /// \brief timestamp copy constructor
  /// \param p_timestamp timestamp to be copied
  ///
  inline timestamp(const timestamp &p_timestamp)
      : m_days(p_timestamp.m_days), m_secs(p_timestamp.m_secs) {}

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  timestamp(timestamp &&p_timestamp)
      : m_days(std::move(p_timestamp.m_days)),
        m_secs(std::move(p_timestamp.m_secs)) {}

  /// \brief operator = copy assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp &operator=(const timestamp &p_timestamp);

  ///
  /// \brief operator = move assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp &operator=(timestamp &&p_timestamp);

  //  ///
  //  /// \brief set_day
  //  /// \param p_day
  //  /// \throw if the resulting \p timestamp is invalid
  //  ///
  //  void set_day(day p_day);

  //  ///
  //  /// \brief set_day_month
  //  /// \param p_day
  //  /// \param p_month
  //  ///
  //  void set_day_month(day p_day, month p_month);

  ///
  /// \brief get_second
  /// \return
  ///
  measures::time::second get_second();

  ///
  /// \brief get_minute
  /// \return
  ///
  measures::time::minute get_minute();

  ///
  /// \brief get_hour
  /// \return
  ///
  measures::time::hour get_hour();

  ///
  /// \brief get_day
  /// \return
  ///
  measures::time::day get_day();

  ///
  /// \brief get_weekday
  /// \return
  ///
  measures::time::weekday get_weekday();

  ///
  /// \brief get_month
  /// \return
  ///
  measures::time::month get_month();

  ///
  /// \brief get_year
  /// \return
  ///
  measures::time::year get_year();

  ///
  /// \brief now
  /// \return
  ///
  static timestamp now();

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  friend std::ostream &operator<<(std::ostream &p_out, timestamp &p_ts);

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  bool operator==(const timestamp &p_ts) const {
    return ((m_days == p_ts.m_days) && (m_secs == p_ts.m_secs));
  }
  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  bool operator!=(const timestamp &p_ts) const {
    return ((m_days != p_ts.m_days) || (m_secs != p_ts.m_secs));
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  bool operator>(const timestamp &p_ts) const;

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  bool operator<(const timestamp &p_ts) const;

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  bool operator>=(const timestamp &p_ts) const;

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  bool operator<=(const timestamp &p_ts) const;

  /// \brief adds an amount of time to the current timestamp
  ///
  /// \tparam t_amount is the type amount of time to be added to this timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be added to this
  /// timestamp
  ///
  /// \return this timestamp modified
  template <typename t_amount> timestamp &operator+=(t_amount p_amount) {
    add(this, p_amount);
    return *this;
  }

  /// \brief adds an amount of time to the current timestamp, creating a new
  /// timestamp
  ///
  /// \tparam t_amount is the type amount of time to be added to this timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be added to this
  /// timestamp
  ///
  /// \return the new timestamp
  template <typename t_amount> timestamp operator+(t_amount p_amount) {
    timestamp _timestamp(*this);
    add(&_timestamp, p_amount);
    return _timestamp;
  }

  /// \brief substractys an amount of time to the current timestamp
  ///
  /// \tparam t_amount is the type amount of time to be substracted to this
  /// timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be substracted to
  /// this timestamp
  ///
  /// \return this timestamp modified
  template <typename t_amount> timestamp &operator-=(t_amount p_amount) {
    substract(this, p_amount);
    return *this;
  }

  /// \brief subtracts an amount of time to the current timestamp, creating a
  /// new timestamp
  ///
  /// \tparam t_amount is the type amount of time to be substracted to this
  /// timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be substracted to
  /// this timestamp
  ///
  /// \return the new timestamp
  template <typename t_amount> timestamp operator-(t_amount p_amount) {
    timestamp _timestamp(*this);
    substract(&_timestamp, p_amount);
    return _timestamp;
  }

private:
  template <typename t_amount> void add(timestamp *p_ts, t_amount p_amount) {
    measures::time::days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    measures::time::seconds _secs_in_day((measures::time::days(_dec)));

    measures::time::seconds _secs(p_ts->m_secs + _secs_in_day);

    if (measures::time::days(_secs) > measures::time::days(1)) {
      p_ts->m_days += measures::time::days(_int + 1);
      p_ts->m_secs = _secs - measures::time::seconds(measures::time::days(1));
    } else {
      p_ts->m_days += measures::time::days(_int);
      p_ts->m_secs += _secs_in_day.integer();
    }
  }

  template <typename t_amount>
  void substract(timestamp *p_ts, t_amount p_amount) {
    measures::time::days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    measures::time::seconds _secs_in_day((measures::time::days(_dec)));

    measures::time::seconds _secs(p_ts->m_secs - _secs_in_day);

    if ( measures::time::days(_secs) < measures::time::days(0)) {
      p_ts->m_days -= measures::time::days(_int + 1);
      p_ts->m_secs = measures::time::seconds(measures::time::days(1)) + _secs;
    } else {
      p_ts->m_days -= measures::time::days(_int);
      p_ts->m_secs -= _secs_in_day.integer();
    }
  }

   void dMyhms2days_secs(measures::time::year p_year, measures::time::month p_month, measures::time::day p_day,
                                  measures::time::hour p_hour, measures::time::minute p_minute,
                                  measures::time::second p_second);

   void days_secs2dMywhms();

private:
  /// \brief amount of days since epoch
  measures::time::days m_days = {0};

  /// \brief amount of seconds in the day
  measures::time::seconds m_secs = {0};

  /// \brief indicates that the parts of the timestamp - m_year, m_month, m_day,
  /// m_hour, m_minute, m_second, m_weekday - need to be updated
  bool m_needs_update = {true};

  measures::time::year m_year = measures::time::year(1970);
  measures::time::month m_month = measures::time::month::jan;
  measures::time::day m_day = measures::time::day::_01;
  measures::time::hour m_hour = measures::time::hour::_00;
  measures::time::minute m_minute = measures::time::minute::_00;
  measures::time::second m_second = measures::time::second::_00;
  measures::time::weekday m_weekday = measures::time::weekday::thu;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif
#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKLY_REPETITION_T_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKLY_REPETITION_T_H

#include <vector>

#include <calendar/gregorian/weekday.h>
#include <calendar/gregorian/weekdays.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {

///
/// \brief Calculates the next group of timestamps, given a set of \p weekday,
/// and the interval between the weeks
///
/// \tparam t_time_precision defines the precision of the t_timestamp. Currently
/// it can be \p second, \p minute, \p weekday, \p hour,\p day, \p month and \p
/// year
///

template <typename t_timestamp> struct weekly_repetition_t {
  typedef std::vector<t_timestamp> timestamps;

  typedef std::vector<_ent::weekday> weekdays;

  ///
  /// \brief weekly_repetition_t
  /// \param p_weekdays
  /// \param p_at_each
  ///
  weekly_repetition_t(weekdays &&p_weekdays, uint16_t p_at_each = 1)
      : m_weekdays(std::move(p_weekdays)), m_at_each(p_at_each) {}

  ///
  /// \brief next
  /// \param p_time
  /// \param p_first
  /// \return
  ///
  timestamps next(t_timestamp p_time, bool p_first = false) {
    timestamps _sequence;
    if (p_first) {
      first(p_time, _sequence);
      return _sequence;
    }

    t_timestamp _time = begin_week(p_time);
    //
    rest(m_weekdays.begin(), _time, _sequence);
    return _sequence;
  }

private:
  ///
  /// \brief begin_week
  /// \param p_time
  /// \return
  ///
  t_timestamp begin_week(t_timestamp p_time) {
    t_timestamp _time = p_time + m_at_each;

    _time -= _time.get_weekday().from_sunday();

    return _time;
  }

  ///
  /// \brief next_timestamp_in_weekday_from_timestamp
  /// \param p_time
  /// \param p_weekday
  /// \return
  ///
  t_timestamp
  next_timestamp_in_weekday_from_timestamp(t_timestamp p_time,
                                           _ent::weekday p_weekday) {
    _ent::weekday _weekday = p_time.get_weekday();

    t_timestamp _time = p_time;

    if (_weekday == p_weekday) {
      return _time;
    }

    if (_weekday < p_weekday) {
      _time += (p_weekday - _weekday);
    } else {
      //      _time += _weekday.until_saturday();
      //      _time += day::amount(1);
      //      _time += p_weekday.from_sunday();
      _time += (_weekday - p_weekday);
    }
    return _time;
  }

  ///
  /// \brief rest
  /// \param p_weekday
  /// \param p_time
  /// \param p_sequence
  ///
  void rest(std::vector<_ent::weekday>::const_iterator p_weekday,
            t_timestamp p_time, timestamps &p_sequence) {
    t_timestamp _time = p_time;
    for (weekdays::const_iterator _ite = p_weekday; _ite != m_weekdays.end();
         ++_ite) {

      _time += (*_ite - _time.get_weekday());

      p_sequence.push_back(_time);
    }
  }

  ///
  /// \brief first
  /// \param p_time
  /// \param p_sequence
  ///
  void first(t_timestamp p_time, timestamps &p_sequence) {
    t_timestamp _time =
        next_timestamp_in_weekday_from_timestamp(p_time, m_weekdays[0]);
    p_sequence.push_back(_time);
    rest(++m_weekdays.begin(), _time, p_sequence);
  }

private:
  ///
  /// \brief m_weekdays
  ///
  weekdays m_weekdays;

  ///
  /// \brief m_at_each
  ///
  _ent::weekdays m_at_each;
};

} // namespace greogorian
} // namespace calendar
} // namespace tenacitas

#endif // weekly_repetition_t_H
#ifndef TENACITAS_CALENDAR_GREGORIAN_YEARLY_REPETITION_T_H
#define TENACITAS_CALENDAR_GREGORIAN_YEARLY_REPETITION_T_H

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/year.h>
#include <calendar/gregorian/years.h>

namespace tenacitas {
namespace calendar {
namespace greogorian {


template<typename t_timestamp>
struct yearly_repetition_t
{
  yearly_repetition_t(_ent::day p_day, _ent::month p_month, uint16_t p_at_each = 1)
    : m_day(p_day)
    , m_month(p_month)
    , m_at_each(p_at_each)
  {}

  t_timestamp next(const t_timestamp &p_time, bool p_first = false)
  {
    t_timestamp _time (p_time);
    if (m_month > _time.get_month()) {
      _time.set_day_month(m_day, m_month);
    } else if (m_month == _time.get_month()) {
      if (m_day > _time.get_day()) {
        _time.set_day(m_day);
      } else if (m_day <= _time.get_day()) {
        // _day < _tm->tm_mday
        _time.set_day(m_day);
      }
    } else {
      _time.set_day_month(m_day, m_month);
    }

    if (!p_first) {
      _time += m_at_each;
    }
    return _time;
  }

private:
  _ent::day m_day;
  _ent::month m_month;
  _ent::years m_at_each;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
#endif

#endif
