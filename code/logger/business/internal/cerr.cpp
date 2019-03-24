#include <logger/business/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace business {

// ----------------------------------------------------------------------------
void
configure_cerr_log()
{
    business::cerr_log_t::configure(business::cerr_media());
}

} // namespace business
} // namespace logger
} // namespace tenacitas
