/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_DATA_SECTION_AT_H
#define TNCT_MF4_PER_DATA_SECTION_AT_H

#include <cstdint>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_at.h"
#include "tnct/mf4/v411/per/data_section.h"

namespace tnct::mf4::v411::per {

template <> struct data_section_t<mem::block_id::AT> {
  mem::data_section_t<mem::block_id::AT> operator()(const std::uint8_t *p_buf,
                                                    std::size_t) {
    using at_flags = std::uint16_t;
    using at_creator_index = std::uint16_t;
    using at_md5_checksum = std::uint8_t;
    using at_original_size = std::uint64_t;
    using at_embedded_size = std::uint64_t;
    using at_embedded_data = std::uint8_t;

    static constexpr std::uint8_t size_at_flags = sizeof(at_flags);
    static constexpr std::uint8_t size_at_creator_index =
        sizeof(at_creator_index);
    static constexpr std::uint8_t size_at_md5_checksum =
        sizeof(at_md5_checksum);

    static constexpr std::uint8_t size_at_original_size =
        sizeof(at_original_size);
    static constexpr std::uint8_t size_at_embedded_size =
        sizeof(at_embedded_size);

    return {byte_array::from_little<at_flags>(p_buf),

            byte_array::from_little<at_creator_index>(p_buf + size_at_flags),

            byte_array::from_little<at_md5_checksum>(p_buf + size_at_flags +
                                                     size_at_creator_index),

            byte_array::from_little<at_original_size>(p_buf + size_at_flags +
                                                      size_at_creator_index +
                                                      size_at_md5_checksum),

            byte_array::from_little<at_embedded_size>(
                p_buf + size_at_flags + size_at_creator_index +
                size_at_md5_checksum + size_at_original_size),

            byte_array::from_little<at_embedded_data>(
                p_buf + size_at_flags + size_at_creator_index +
                size_at_md5_checksum + size_at_original_size +
                size_at_embedded_size)};
  };
};
} // namespace tnct::mf4::v411::per

#endif
