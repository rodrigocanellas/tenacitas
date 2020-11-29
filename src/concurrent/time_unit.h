#ifndef TENACITAS_CONCURRENT_TIME_UNIT_H
#define TENACITAS_CONCURRENT_TIME_UNIT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Type of time used to define timeout
typedef std::chrono::microseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::microseconds interval;


/// \brief Converts any type of time defined in std::chrono to \p timeout
template <typename t_time> inline timeout to_timeout(t_time p_time) {
  return std::chrono::duration_cast<timeout>(p_time);
}

/// \brief Converts any type of time defined in std::chrono to \p interval
template <typename t_time> inline interval to_interval(t_time p_time) {
  return std::chrono::duration_cast<interval>(p_time);
}

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TIME_UNIT_H
