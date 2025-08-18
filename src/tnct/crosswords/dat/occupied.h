#ifndef TNCT_CROSSWORDS_DAT_OCCUPIED_H
#define TNCT_CROSSWORDS_DAT_OCCUPIED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/container/matrix.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/dat/word.h"

namespace tnct::crosswords::dat
{

/// \brief Defines which coordinates are occupied
using occupied = tnct::container::matrix<crosswords::dat::index,
                                         crosswords::dat::word::value_type>;

} // namespace tnct::crosswords::dat

#endif
