#include <logger/cout/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace cout {

bus::log_t<log> log::m_log([](std::string &&p_str) { std::cout << p_str; });

} // namespace cout
} // namespace logger
} // namespace tenacitas
