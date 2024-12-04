/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_FORMAT_FMT_H
#define TENACITAS_LIB_FORMAT_FMT_H

#include <sstream>

#include "tenacitas.lib/pair/pair_output.h"
#include "tenacitas.lib/time/chrono_output.h"
#include "tenacitas.lib/tuple/tuple_output.h"

namespace tenacitas::lib::format {

/// \brief Formats the params into a string
template <typename... t_params>
static inline std::string fmt(t_params &&...p_params) {
  std::ostringstream _stream;
  ((_stream << p_params), ...);
  return _stream.str();
}

} // namespace tenacitas::lib::format

#endif
