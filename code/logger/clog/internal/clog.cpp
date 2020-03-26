#include <logger/clog/clog.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

namespace clog {

bus::t_log<log> log::m_log([](std::string &&p_str) { std::clog << p_str; });

} // namespace clog
} // namespace logger
} // namespace tenacitas
