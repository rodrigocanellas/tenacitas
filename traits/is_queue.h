/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_IS_QUEUE_H
#define TENACITAS_LIB_TRAITS_IS_QUEUE_H

#include <tenacitas.lib/traits/queue.h>

namespace tenacitas::lib::traits {

template <typename t_queue>
concept is_queue = queue<t_queue, typename t_queue::data>;

} // namespace tenacitas::lib::traits

#endif
