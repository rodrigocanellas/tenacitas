/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_CH_H
#define TNCT_MF4_PER_DATA_SECTION_CH_H

#include <cstdint>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_ch.h"
#include "tnct/mf4/v411/per/data_section.h"

namespace tnct::mf4::v411::per {

template <> struct data_section_t<mem::block_id::CH> {
  mem::data_section_t<mem::block_id::CH> operator()(const std::uint8_t *p_buf,
                                                    std::size_t) {
    using ch_element_count = std::uint32_t;
    using ch_type = std::uint8_t;

    static constexpr std::uint32_t size_ch_element_count{
        sizeof(ch_element_count)};

    return {byte_array::from_little<ch_element_count>(p_buf),

            byte_array::from_little<ch_type>(p_buf + size_ch_element_count)};
  }
};
} // namespace tnct::mf4::v411::per

#endif
