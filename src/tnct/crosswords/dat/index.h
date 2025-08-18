#ifndef TNCT_CROSSWORDS_DAT_INDEX_H
#define TNCT_CROSSWORDS_DAT_INDEX_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>

namespace tnct::crosswords::dat
{

using index = std::int16_t;

static constexpr index max_row{1000};
static constexpr index max_col{1000};

static constexpr index invalid_index{-1};

} // namespace tnct::crosswords::dat

#endif
