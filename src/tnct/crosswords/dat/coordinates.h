#ifndef TENACITAS_SRC_CROSSWORDS_DAT_COORDINATES_H
#define TENACITAS_SRC_CROSSWORDS_DAT_COORDINATES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <vector>

#include "tenacitas/src/crosswords/dat/coordinate.h"

namespace tenacitas::src::crosswords::dat {

/// \brief Set of rows and columns
using coordinates = std::vector<coordinate>;

} // namespace tenacitas::src::crosswords::dat

#endif
