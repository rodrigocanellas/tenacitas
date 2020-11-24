#ifndef TENACITAS_CONCURRENT_CONSTANTS_H
#define TENACITAS_CONCURRENT_CONSTANTS_H

#include <chrono>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

static constexpr std::chrono::nanoseconds infinite_timeout{
    std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::hours(24 * 30 * 12))};

} // namespace concurrent
} // namespace tenacitas

#endif
