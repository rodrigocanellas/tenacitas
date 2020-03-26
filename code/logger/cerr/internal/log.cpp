#include <logger/cerr/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace cerr {

bus::t_log<log> log::m_log([](std::string &&p_str) { std::cerr << p_str; });

} // namespace cerr
} // namespace logger
} // namespace tenacitas
