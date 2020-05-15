#include <logger/clog/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

namespace clog {

log_t<log> log::m_log([](std::string &&p_str) { std::clog << p_str; });

} // namespace clog
} // namespace logger
} // namespace tenacitas
