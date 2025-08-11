#ifndef TENACITAS_SRC_CROSSWORDS_DAT_COORDINATE_H
#define TENACITAS_SRC_CROSSWORDS_DAT_COORDINATE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <utility>

#include "tenacitas/src/crosswords/dat/index.h"

namespace tenacitas::src::crosswords::dat {

/// \brief Row and Column
using coordinate = std::pair<index, index>;

} // namespace tenacitas::src::crosswords::dat

#endif
