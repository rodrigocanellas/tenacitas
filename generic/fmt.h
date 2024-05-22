/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_GENERIC_FMT_H
#define TENACITAS_LIB_GENERIC_FMT_H

#include <tenacitas.lib/generic/chrono_output.h>
#include <tenacitas.lib/generic/tuple_output.h>

namespace tenacitas::lib::generic {

/// \brief Formats the params into a string
template <typename... t_params>
static inline std::string fmt(t_params &&...p_params) {
  std::ostringstream _stream;
  ((_stream << p_params), ...);
  return _stream.str();
}

} // namespace tenacitas::lib::generic

#endif
