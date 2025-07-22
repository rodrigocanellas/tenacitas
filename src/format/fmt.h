/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_FORMAT_FMT_H
#define TENACITAS_SRC_FORMAT_FMT_H

#include <sstream>

/// \attention the headers below are necessary, although not explicitly used,
/// because of the 'operator<<' that they define

#include "tenacitas/src/ostream/pair.h"
#include "tenacitas/src/ostream/tuple.h"
#include "tenacitas/src/time/chrono_output.h"

namespace tenacitas::src::format {

/// \brief Formats the params into a string
template <typename... t_params>
static inline std::string fmt(t_params &&...p_params) {
  std::ostringstream _stream;
  ((_stream << p_params), ...);
  return _stream.str();
}

} // namespace tenacitas::src::format

#endif
