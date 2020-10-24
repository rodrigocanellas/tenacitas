#ifndef TENACITAS_CONCURRENT_TIMEOUT_CALLBACK_H
#define TENACITAS_CONCURRENT_TIMEOUT_CALLBACK_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <thread>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

typedef std::function<void(std::thread::id)> timeout_callback;

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
