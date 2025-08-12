/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_COUT_H
#define TNCT_LOG_COUT_H

#include "tnct/log/internal/logger.h"

namespace tnct::log
{

using cout = tnct::log::internal::logger<decltype([](std::string_view s)
                                                  { std::cout << s; })>;

} // namespace tnct::log

#endif
