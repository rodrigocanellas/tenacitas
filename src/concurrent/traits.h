#ifndef TENACITAS_CONCURRENT_TRAITS_H
#define TENACITAS_CONCURRENT_TRAITS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <concurrent/circular_fixed_size_queue.h>
#include <concurrent/circular_unlimited_size_queue.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_data> struct traits {
  typedef circular_unlimited_size_queue_t<t_log, t_data> queue;
};

} // namespace concurrent
} // namespace tenacitas

#endif
