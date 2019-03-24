#ifndef TENACITAS_LOGGER_BUS_CONFIGURE_COUT_LOG_H
#define TENACITAS_LOGGER_BUS_CONFIGURE_COUT_LOG_H

#include <logger/business/internal/cout_media.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

///
/// \brief cerr_log_t defines the type of log specialized for cout
///
typedef business::log<business::cout_media> cout_log_t;

} // namespace business
} // namespace logger
} // namespace tenacitas

#endif // CONFIGURE_COUT_LOG_H
