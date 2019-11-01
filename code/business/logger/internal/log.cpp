#include <business/logger/log.h>

namespace tenacitas {
namespace business {
namespace logger {

logger::log::writer logger::log::m_writer = [](std::string&&) {};

level logger::log::m_level = { level::error };

char logger::log::m_separator = { '|' };

std::mutex logger::log::m_mutex;

} // namespace logger
} // namespace business
} // namespace tenacitas
