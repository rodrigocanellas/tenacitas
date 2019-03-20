#ifndef TENACITAS_LOGGER_BUS_CONFIGURE_DEFAULT_FILE_LOG_H
#define TENACITAS_LOGGER_BUS_CONFIGURE_DEFAULT_FILE_LOG_H

#include <logger/bus/internal/file_controller.h>
#include <logger/bus/internal/file_media.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace bus {

/// \brief file_controller_t is just an alias for default_file_controller
typedef bus::file_controller file_controller_t;

/// \brief file_media_t is just an alias for \p file_media
typedef bus::file_media<file_controller_t> file_media_t;

/// \brief file_log_t defines the type of log specialized for file
typedef bus::log<file_media_t> file_log_t;

} // namespace bus
} // namespace logger
} // namespace tenacitas

#endif // CONFIGURE_DEFAULT_FILE_LOG_H
