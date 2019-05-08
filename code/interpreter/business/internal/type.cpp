
#include <interpreter/business/internal/type.h>

namespace tenacitas {
namespace interpreter {
namespace business {

const type type::unreconized_type("--ur--", [](const std::string&) -> bool {
    return false;
});

} // namespace business
} // namespace interpreter
} // namespace tenacitas
