/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_HD_H
#define TNCT_MF4_PER_DATA_SECTION_HD_H

#include <cstdint>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_hd.h"
#include "tnct/mf4/v411/per/data_section.h"

namespace tnct::mf4::v411::per {

template <> struct data_section_t<mem::block_id::HD> {
  mem::data_section_t<mem::block_id::HD> operator()(const std::uint8_t *p_buf,
                                                    std::size_t) {

    using hd_start_time_ns = std::uint64_t;
    using hd_tz_offset_min = std::int16_t;
    using hd_dst_offset_min = std::int16_t;
    using hd_time_flags = std::uint8_t;
    using hd_time_class = std::uint8_t;
    using hd_flags = std::uint8_t;
    using reserved = std::uint8_t;
    using hd_start_angle_rad = double;
    using hd_start_angle_m = double;

    static constexpr std::uint8_t size_hd_start_time_ns{
        sizeof(hd_start_time_ns)};
    static constexpr std::uint8_t size_hd_tz_offset_min{
        sizeof(hd_tz_offset_min)};
    static constexpr std::uint8_t size_hd_dst_offset_min{
        sizeof(hd_dst_offset_min)};
    static constexpr std::uint8_t size_hd_time_flags{sizeof(hd_time_flags)};
    static constexpr std::uint8_t size_hd_time_class{sizeof(hd_time_class)};
    static constexpr std::uint8_t size_hd_flags{sizeof(hd_flags)};
    static constexpr std::uint8_t size_reserved{sizeof(reserved)};
    static constexpr std::uint8_t size_hd_start_angle_rad{
        sizeof(hd_start_angle_rad)};

    return {
        byte_array::from_little<hd_start_time_ns>(p_buf),

        byte_array::from_little<hd_tz_offset_min>(p_buf +
                                                  size_hd_start_time_ns),

        byte_array::from_little<hd_dst_offset_min>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min),

        byte_array::from_little<hd_time_flags>(p_buf + size_hd_start_time_ns +
                                               size_hd_tz_offset_min +
                                               size_hd_dst_offset_min),

        byte_array::from_little<hd_time_class>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags),

        byte_array::from_little<hd_flags>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class),

        byte_array::from_little<hd_start_angle_rad>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class +
            size_hd_flags + size_reserved),

        byte_array::from_little<hd_start_angle_m>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class +
            size_hd_flags + size_reserved + size_hd_start_angle_rad)};
  }
};

} // namespace tnct::mf4::v411::per

#endif
