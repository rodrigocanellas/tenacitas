#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace bus {

// ----------------------------------------------------------------------------
void
configure_cerr_log()
{
    bus::cerr_log_t::configure(bus::cerr_media());
}

} // namespace bus
} // namespace logger
} // namespace tenacitas
