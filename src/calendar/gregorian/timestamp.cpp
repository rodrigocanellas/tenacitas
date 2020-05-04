#include <calendar/gregorian/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

timestamp::timestamp() { now(); }

void timestamp::now() {
  typedef std::chrono::duration<
      int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>>
      days_t;
  auto _now = std::chrono::system_clock::now().time_since_epoch();
  auto _today = std::chrono::duration_cast<days_t>(_now);
  m_days = days(_today.count());
  _now -= _today;
  m_secs =
      seconds(std::chrono::duration_cast<std::chrono::seconds>(_now).count());

  days_secs2dMywhms();
}

timestamp::timestamp(year p_year, month p_month, day p_day, hour p_hour,
                     minute p_minute, second p_second) {
  dMyhms2days_secs(p_year, p_month, p_day, p_hour, p_minute, p_second);
  days_secs2dMywhms();
}

constexpr void timestamp::dMyhms2days_secs(year p_year, month p_month,
                                           day p_day, hour p_hour,
                                           minute p_minute, second p_second) {

  int64_t _years = p_year.value();
  const auto _months = p_month.value();
  const auto _days = p_day.value();
  _years -= _months <= 2;
  const auto era = (_years >= 0 ? _years : _years - 399) / 400;
  const unsigned yoe = static_cast<unsigned>(_years - era * 400); // [0, 399]
  const unsigned doy = (153 * (_months + (_months > 2 ? -3 : 9)) + 2) / 5 +
                       _days - 1;                             // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
  m_days = days(era * 146097 + static_cast<decltype(_years)>(doe) - 719468);
  m_secs = seconds(hours(p_hour)) + seconds(minutes(p_minute)) + p_second;
}

constexpr void timestamp::days_secs2dMywhms() {
  int64_t _days(m_days.value());
  _days += 719468;
  const int64_t era = (_days >= 0 ? _days : _days - 146096) / 146097;
  const auto doe = static_cast<unsigned>(_days - era * 146097); // [0, 146096]
  const auto yoe =
      (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
  auto y = static_cast<int32_t>(yoe) + era * 400;
  const auto doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
  const auto mp = (5 * doy + 2) / 153;                      // [0, 11]
  const auto d = doy - (153 * mp + 2) / 5 + 1;              // [1, 31]
  const auto m = mp + (mp < 10 ? 3 : -9);                   // [1, 12]
  m_year = year(y + (m <= 2));
  m_month = month(m);
  m_day = day(d);
  m_weekday = weekday(_days >= -4 ? (_days + 4) % 7 : (_days + 5) % 7 + 6);
  m_hour = hours(m_secs);
  m_minute = minutes(m_secs - seconds((hours(m_hour))));
  m_second = seconds(m_secs % 60);
}

std::ostream &operator<<(std::ostream &p_out, const timestamp &p_ts) {
  p_out << std::setw(4) << std::setfill('0') << p_ts.get_year() << "-"

        << std::setw(2) << std::setfill('0') << p_ts.get_month().value() << "-"

        << std::setw(2) << std::setfill('0') << p_ts.get_day() << " "

        << p_ts.get_weekday() << " "

        << std::setw(2) << std::setfill('0') << p_ts.get_hour() << ":"

        << std::setw(2) << std::setfill('0') << p_ts.get_minute() << ":"

        << std::setw(2) << std::setfill('0') << p_ts.get_second()

      ;

  return p_out;
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
