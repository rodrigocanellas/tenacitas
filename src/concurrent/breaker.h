#ifndef TENACITAS_CONCURRENT_BREAKER_H
#define TENACITAS_CONCURRENT_BREAKER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <thread>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
