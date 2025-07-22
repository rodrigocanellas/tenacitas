/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_COUT_LOG_H
#define TNCT_LOG_COUT_LOG_H

#include "tenacitas/src/log/internal/logger.h"

namespace tenacitas::src::log {

using cout = tenacitas::src::log::internal::logger<decltype(
    [](std::string_view s) { std::cout << s; })>;

} // namespace tenacitas::src::log

#endif
