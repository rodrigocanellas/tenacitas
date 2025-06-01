/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_H
#define TNCT_MF4_PER_DATA_SECTION_H

#include <cstdint>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

template <mem::block_id t_block_id> struct data_section_t {
  template <traits::log::logger t_logger>
  mem::data_section_t<t_block_id> operator()(t_logger &p_logger,
                                             const std::uint8_t *p_buf,
                                             std::size_t p_size) {
    return mem::data_section_t<t_block_id>(p_logger, p_buf, p_size);
  }
};

} // namespace tnct::mf4::v411::per

#endif
