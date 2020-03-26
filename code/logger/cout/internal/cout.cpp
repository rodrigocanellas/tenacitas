#include <logger/cout/cout.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace cout {

bus::t_log<log> log::m_log([](std::string &&p_str) { std::cout << p_str; });

} // namespace cout
} // namespace logger
} // namespace tenacitas
