#include <logger/cerr/cerr.h>

// \brief namespace of the organization
namespace tenacitas {
// \brief namespace of the project
namespace logger {

bus::log_t<cerr> cerr::m_log([](std::string &&p_str) { std::cerr << p_str; });
} // namespace logger
} // namespace tenacitas
