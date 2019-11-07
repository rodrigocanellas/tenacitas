#include <logger/business/log.h>

namespace tenacitas {
namespace logger {
namespace business {

log::writer log::m_writer = [](std::string&&) {};

level log::m_level = { level::error };

char log::m_separator = { '|' };

std::mutex log::m_mutex;

} // namespace logger
} // namespace business
} // namespace tenacitas
