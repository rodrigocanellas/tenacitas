#ifndef TENACITAS_SRC_CROSSWORDS_DAT_OCCUPIED_H
#define TENACITAS_SRC_CROSSWORDS_DAT_OCCUPIED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/crosswords/dat/word.h"
#include "tenacitas/src/container/matrix.h"

namespace tenacitas::src::crosswords::dat {

/// \brief Defines which coordinates are occupied
using occupied = tenacitas::src::container::matrix<index, word::value_type>;

} // namespace tenacitas::src::crosswords::dat

#endif
