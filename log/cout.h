/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_LOG_COUT_LOG_H
#define TENACITAS_LIB_LOG_COUT_LOG_H

#include <tenacitas.lib/log/internal/logger.h>

namespace tenacitas::lib::log {

using cout = tenacitas::lib::log::internal::logger<decltype(
    [](std::string_view s) { std::cout << s; })>;

} // namespace tenacitas::lib::log

#endif
