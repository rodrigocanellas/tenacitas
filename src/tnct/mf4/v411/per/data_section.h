/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_H
#define TNCT_MF4_PER_DATA_SECTION_H

#include <cstdint>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::per {

template <mem::block_id t_block_id> struct data_section_t {
  mem::data_section_t<t_block_id> operator()(const std::uint8_t *,
                                             std::size_t) {
    return {};
  }
};

} // namespace tnct::mf4::v411::per

#endif
