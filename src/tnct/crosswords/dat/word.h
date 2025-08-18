#ifndef TNCT_CROSSWORDS_DAT_WORD_H
#define TNCT_CROSSWORDS_DAT_WORD_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <limits>
#include <string>

namespace tnct::crosswords::dat
{

using word = std::string;

static constexpr word::value_type max_char{
    std::numeric_limits<word::value_type>::max()};

} // namespace tnct::crosswords::dat

#endif
