/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_CERR_LOG_H
#define TNCT_LOG_CERR_LOG_H

#include <string_view>

#include "tnct/log/internal/logger.h"

namespace tnct::log {

struct cerr_writer {
  void operator()(std::string_view p_str) { std::cerr << p_str; }
};

using cerr = tnct::log::internal::logger<cerr_writer>;

} // namespace tnct::log

#endif
