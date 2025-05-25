/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_BASIC_TYPES_H
#define TNCT_MF4_PER_BASIC_TYPES_H

#include <cstdint>

namespace tnct::mf4::v411::per {
using amount_to_read = std::uint64_t;
using offset = std::int64_t;
using field_size = std::uint8_t;

static constexpr amount_to_read header_section_size{24};

} // namespace tnct::mf4::v411::per

#endif
