#ifndef TENACITAS_SRC_CROSSWORDS_DAT_INDEX_H
#define TENACITAS_SRC_CROSSWORDS_DAT_INDEX_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <limits>

namespace tenacitas::src::crosswords::dat {

/// \brief Index in a grid
using index = int16_t;

static constexpr index max_row{std::numeric_limits<index>::max()};
static constexpr index max_col{std::numeric_limits<index>::max()};

} // namespace tenacitas::src::crosswords::dat

#endif
