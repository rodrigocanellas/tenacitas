#include <calendar/gregorian/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

timestamp::timestamp() {

  typedef std::chrono::duration<
      int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>>
      days_t;
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto today = std::chrono::duration_cast<days_t>(now);
  m_days = days(today.count());
  now -= today;
  m_secs =
      seconds(std::chrono::duration_cast<std::chrono::seconds>(now).count());

  days2mdy();
  secs2hms();
}

// constexpr void timestamp::dmy2days(year p_year, month p_month, day p_day) {

//  int32_t _years = p_year.value();
//  int32_t _months = p_month.value();
//  int32_t _days = p_day.value();
//  _years -= _months <= 2;
//  const int64_t era = (_years >= 0 ? _years : _years - 399) / 400;
//  const unsigned yoe = static_cast<unsigned>(_years - era * 400); // [0, 399]
//  const unsigned doy = (153 * (_months + (_months > 2 ? -3 : 9)) + 2) / 5 +
//                       _days - 1;                             // [0, 365]
//  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
//  m_days = days(era * 146097 + static_cast<decltype(_years)>(doe) - 719468);
//}

constexpr void timestamp::dmy2days(year p_year, month p_month, day p_day) {

  int32_t _years = p_year.value();
  const int32_t _months = p_month.value();
  const int32_t _days = p_day.value();
  _years -= _months <= 2;
  const int64_t era = (_years >= 0 ? _years : _years - 399) / 400;
  const unsigned yoe = static_cast<unsigned>(_years - era * 400); // [0, 399]
  const unsigned doy = (153 * (_months + (_months > 2 ? -3 : 9)) + 2) / 5 +
                       _days - 1;                             // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
  m_days = days(era * 146097 + static_cast<decltype(_years)>(doe) - 719468);
}

constexpr void timestamp::days2mdy() {
  auto _days(m_days.value());
  _days += 719468;
  const int64_t era = (_days >= 0 ? _days : _days - 146096) / 146097;
  const unsigned doe =
      static_cast<unsigned>(_days - era * 146097); // [0, 146096]
  const unsigned yoe =
      (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
  int32_t y = static_cast<int32_t>(yoe) + era * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
  const unsigned mp = (5 * doy + 2) / 153;                      // [0, 11]
  const unsigned d = doy - (153 * mp + 2) / 5 + 1;              // [1, 31]
  const unsigned m = mp + (mp < 10 ? 3 : -9);                   // [1, 12]
  m_year = year(y + (m <= 2));
  m_month = month(m);
  m_day = day(d);
}

constexpr void timestamp::secs2hms() {
  m_hour = hours(m_secs);
  m_minute = minutes(m_secs - seconds((hours(m_hour))));
  m_second = seconds(m_secs % 60);
}

std::ostream &operator<<(std::ostream &p_out, const timestamp &p_ts) {
  p_out << "["

        << std::setw(2) << std::setfill('0') << p_ts.get_day() << "/"

        << std::setw(2) << std::setfill('0') << p_ts.get_month() << "/"

        << std::setw(4) << std::setfill('0') << p_ts.get_year()
        << ","

        //          << p_ts.get_weekday() << ","

        << std::setw(2) << std::setfill('0') << p_ts.get_hour() << ":"

        << std::setw(2) << std::setfill('0') << p_ts.get_minute() << ":"

        << std::setw(2) << std::setfill('0') << p_ts.get_second()

        << "]";

  //    p_out << "{ "

  //          << "\"day\" : \"" << std::setw(2) << std::setfill('0')
  //          << p_ts.get_day() << "\", "

  //          << "\"month\" : \"" << std::setw(2) << std::setfill('0')
  //          << p_ts.get_month() << "\", "

  //          << "\"year\" : \"" << std::setw(4) << std::setfill('0')
  //          << p_ts.get_year() << "\", "

  //          << "\"weekday\" : \"" << p_ts.get_weekday() << "\", "

  //          << "\"hour\" : \"" << std::setw(2) << std::setfill('0')
  //          << p_ts.get_hour() << "\", "

  //          << "\"minute\" : \"" << std::setw(2) << std::setfill('0')
  //          << p_ts.get_minute() << "\", "

  //          << "\"second\" : \"" << std::setw(2) << std::setfill('0')
  //          << p_ts.get_second() << "\" "

  //          << " }";

  return p_out;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
