/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_DT_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_DT_H

#include <iostream>

// #include "tnct/byte_array/classes.h"
#include "tnct/format/fmt.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::DT> final {

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t * /*p_buf*/,
                 std::size_t p_size) {
    TNCT_LOG_DEB(p_logger, format::fmt("size of DT = ", p_size));

    // byte_array::from_buffer_little_endian(
    //     p_logger, p_buf, _sizeof, std::source_location::current(), m_ca_type,
    //     m_ca_storage, m_ca_flags, m_ca_ref_count, m_ca_val_count,
    //     m_ca_phy_range_min, m_ca_phy_range_max);

    // if (m_ca_val_count != 0) {
    //   m_ca_vals = ca_vals(p_size - _sizeof, ca_val{0.0});

    //   for (ca_val_count _i = 0; _i < m_ca_val_count; ++_i) {
    //     m_ca_vals[_i] = byte_array::from_little<ca_val>(p_buf + p_size +
    //                                                     (_i *
    //                                                     sizeof(ca_val)));
    //   }
    // }
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::DT> & /*p_obj*/) {
  p_out << "{"
        << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
