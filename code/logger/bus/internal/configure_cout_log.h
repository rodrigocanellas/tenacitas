#ifndef TENACITAS_LOGGER_BUS_CONFIGURE_COUT_LOG_H
#define TENACITAS_LOGGER_BUS_CONFIGURE_COUT_LOG_H

#include <logger/bus/internal/cout_media.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace bus {

///
/// \brief cerr_log_t defines the type of log specialized for cout
///
typedef bus::log<bus::cout_media> cout_log_t;

} // namespace bus
} // namespace logger
} // namespace tenacitas

#endif // CONFIGURE_COUT_LOG_H
