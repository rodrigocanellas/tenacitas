/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_LOG_AND_THROW_H
#define TNCT_MF4_V411_LOG_AND_THROW_H

#include <source_location>
#include <stdexcept>

#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411

{
template <tnct::traits::log::logger t_logger>
void log_and_throw(
    t_logger &p_logger, std::string_view p_str,
    std::source_location p_location = std::source_location::current()) {
  p_logger.err(p_str, p_location);
  throw std::runtime_error(p_str.data());
}

} // namespace tnct::mf4::v411
#endif
