#include <logger/internal/log.h>
namespace tenacitas {
namespace logger {

bool log::can_log(level p_level) const {
  if (m_level == level::no_log) {
    return false;
  }
  if ((p_level == level::error) || (p_level == level::fatal)) {
    return true;
  }
  return ((p_level >= m_level) || (p_level >= get_level()));
}

std::string log::now() const {

  using namespace std;
  using namespace chrono;

  const auto _microsecs = microseconds();
  const auto _duration = std::chrono::microseconds(_microsecs);
  const time_point<system_clock> _time_point(_duration);
  const time_t _time_t = system_clock::to_time_t(_time_point);

  const auto _remainder = _microsecs % 1000000;
  stringstream _stream;
  _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
          << std::setw(6) << std::setfill('0') << std::left << _remainder;
  return _stream.str();
}

} // namespace logger
} // namespace tenacitas
