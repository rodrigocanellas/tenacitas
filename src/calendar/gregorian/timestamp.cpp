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
  m_secs = std::chrono::duration_cast<std::chrono::seconds>(now).count();

  ymd2n(m_days);

  /*
$total = 685;
$horas = floor($total / 3600);
$minutos = floor(($total - ($horas * 3600)) / 60);
$segundos = floor($total % 60);
echo $horas . ":" . $minutos . ":" . $segundos;
*/
  uint16_t _h = static_cast<uint16_t>(m_secs / 3600);
  uint16_t _m = static_cast<uint16_t>((m_secs - (_h * 3600)) / 60);
  uint16_t _s = (m_secs % 60);

  m_hour = hour::create(_h);
  m_minute = minute::create(_m);
  m_second = second::create(_s);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
