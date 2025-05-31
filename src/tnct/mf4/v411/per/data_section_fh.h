/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_FH_H
#define TNCT_MF4_PER_DATA_SECTION_FH_H

#include <cstdint>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_fh.h"
#include "tnct/mf4/v411/per/data_section.h"

namespace tnct::mf4::v411::per {

template <> struct data_section_t<mem::block_id::FH> {
  mem::data_section_t<mem::block_id::FH> operator()(const std::uint8_t *p_buf,
                                                    std::size_t) {
    using fh_time_ns = std::uint64_t;
    using fh_tz_offset_min = std::int16_t;
    using fh_dst_offset_min = std::int16_t;
    using fh_time_flags = std::uint8_t;

    static constexpr std::uint8_t size_fh_time_ns{sizeof(fh_time_ns)};
    static constexpr std::uint8_t size_fh_tz_offset_min{
        sizeof(fh_tz_offset_min)};
    static constexpr std::uint8_t size_fh_dst_offset_min{
        sizeof(fh_dst_offset_min)};

    return {byte_array::from_little<fh_time_ns>(p_buf),

            byte_array::from_little<fh_tz_offset_min>(p_buf + size_fh_time_ns),

            byte_array::from_little<fh_dst_offset_min>(p_buf + size_fh_time_ns +
                                                       size_fh_tz_offset_min),

            byte_array::from_little<fh_time_flags>(p_buf + size_fh_time_ns +
                                                   size_fh_tz_offset_min +
                                                   size_fh_dst_offset_min)};
  }
};
} // namespace tnct::mf4::v411::per

#endif
