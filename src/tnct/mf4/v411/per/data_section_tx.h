/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_TX_H
#define TNCT_MF4_PER_DATA_SECTION_TX_H

#include <cstdint>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_tx.h"
#include "tnct/mf4/v411/per/data_section.h"

namespace tnct::mf4::v411::per {

template <> struct data_section_t<mem::block_id::TX> {
  mem::data_section_t<mem::block_id::TX> operator()(const std::uint8_t *p_buf,
                                                    std::size_t p_size) {

    return {std::string(reinterpret_cast<const char *>(p_buf), p_size - 1)};
  }
};

} // namespace tnct::mf4::v411::per

#endif
