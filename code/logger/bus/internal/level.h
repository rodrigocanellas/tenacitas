#ifndef TENACITAS_LOGGER_BUS_LEVEL_H
#define TENACITAS_LOGGER_BUS_LEVEL_H

#include <cstdint>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace bus {

///
/// \brief The level enum defines the possible log levels
///
enum class level : char
{
    debug = 'D',
    test = 'T',
    info = 'I',
    warn = 'W',
    error = 'E',
    fatal = 'F'
};

} // namespace bus
} // namespace logger
} // namespace tenacitas

#endif
