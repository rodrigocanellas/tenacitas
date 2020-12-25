#include <logger/level.h>

namespace tenacitas {
namespace logger {

level global_level{level::warn};

void set_debug_level() { global_level = level::debug; }
void set_info_level() { global_level = level::info; }
void set_warn_level() { global_level = level::warn; }

bool can_log(level p_level) { return p_level >= global_level; }

} // namespace logger
} // namespace tenacitas
