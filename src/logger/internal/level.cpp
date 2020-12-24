#include <logger/level.h>

namespace tenacitas {
namespace logger {

level global_level{level::warn};

level get_level() { return global_level; }

void set_level(level p_level) { global_level = p_level; }

} // namespace logger
} // namespace tenacitas
