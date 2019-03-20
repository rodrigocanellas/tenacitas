#include <logger/bus/cout.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace bus {

// ----------------------------------------------------------------------------
void
configure_cout_log()
{
    bus::cout_log_t::configure(bus::cout_media());
}

} // namespace bus
} // namespace logger
} // namespace tenacitas
