#include <logger/business/cout.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace business {

// ----------------------------------------------------------------------------
void
configure_cout_log()
{
    business::cout_log_t::configure(business::cout_media());
}

} // namespace business
} // namespace logger
} // namespace tenacitas
