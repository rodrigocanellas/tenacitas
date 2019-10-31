#include <business/logger/log.h>

namespace tenacitas {
namespace business {
namespace logger {

log::writer log::m_writer = [](std::string&&) {};

level log::m_level = { level::error };

char log::m_separator = { '|' };

std::mutex log::m_mutex;

} // namespace logger
} // namespace business
} // namespace tenacitas
