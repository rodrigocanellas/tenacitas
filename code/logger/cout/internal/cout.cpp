#include <logger/cout/cout.h>

// \brief namespace of the organization
namespace tenacitas {
// \brief namespace of the project
namespace logger {

bus::log_t<cout> cout::m_log([](std::string &&p_str) { std::cout << p_str; });

} // namespace logger
} // namespace tenacitas
