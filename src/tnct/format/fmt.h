/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_FORMAT_FMT_H
#define TNCT_FORMAT_FMT_H

#include <sstream>

/// \attention the headers below are necessary, although not explicitly used,
/// because of the 'operator<<' that they define

#include "tnct/ostream/pair.h"
#include "tnct/ostream/tuple.h"
#include "tnct/time/output.h"

namespace tnct::format
{

/// \brief Formats the params into a string
template <typename... t_params>
static inline std::string fmt(t_params &&...p_params)
{
  std::ostringstream _stream;
  ((_stream << p_params), ...);
  return _stream.str();
}

} // namespace tnct::format

#endif
