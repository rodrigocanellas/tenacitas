#ifndef TENACITAS_CONCURRENT_CONSTANTS_H
#define TENACITAS_CONCURRENT_CONSTANTS_H

#include <chrono>

#include <concurrent/timeout.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Defines a infinite timeout, used when no timeout is defined
static constexpr timeout infinite_timeout{
    std::chrono::duration_cast<timeout>(std::chrono::hours(24 * 30 * 12))};

} // namespace concurrent
} // namespace tenacitas

#endif
