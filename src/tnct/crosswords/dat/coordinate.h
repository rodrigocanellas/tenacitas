#ifndef TNCT_CROSSWORDS_DAT_COORDINATE_H
#define TNCT_CROSSWORDS_DAT_COORDINATE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <utility>

#include "tnct/crosswords/dat/index.h"

namespace tnct::crosswords::dat {

/// \brief Row and Column
using coordinate = std::pair<index, index>;

} // namespace tnct::crosswords::dat

#endif
