#include <logger/cerr/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace cerr {

bus::log_t<log> log::m_log([](std::string &&p_str) -> void {
  std::cerr << p_str;
});

} // namespace cerr
} // namespace logger
} // namespace tenacitas
