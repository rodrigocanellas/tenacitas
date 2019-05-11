
#include <interpreter/business/internal/type.h>

namespace tenacitas {
namespace interpreter {
namespace business {

const type
type::unreconized(type::id(""),
                       [](const std::string&) -> bool { return false; });

} // namespace business
} // namespace interpreter
} // namespace tenacitas
