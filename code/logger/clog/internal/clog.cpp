#include <logger/clog/clog.h>

// \brief namespace of the organization
namespace tenacitas {
// \brief namespace of the project
namespace logger {

bus::log_t<clog> clog::m_log([](std::string &&p_str) { std::clog << p_str; });

} // namespace logger
} // namespace tenacitas
