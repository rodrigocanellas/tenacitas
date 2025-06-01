/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_FH_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_FH_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::FH> final {

  using fh_time_ns = std::uint64_t;
  using fh_tz_offset_min = std::int16_t;
  using fh_dst_offset_min = std::int16_t;
  using fh_time_flags = std::uint8_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(), m_fh_time_ns,
        m_fh_tz_offset_min, m_fh_dst_offset_min, m_fh_time_flags, m_reserved_1,
        m_reserved_2, m_reserved_3);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  fh_time_ns get_fh_time_ns() const { return m_fh_time_ns; }

  fh_tz_offset_min get_fh_tz_offset_min() const { return m_fh_tz_offset_min; }

  fh_dst_offset_min get_fh_dst_offset_min() const {
    return m_fh_dst_offset_min;
  }

  fh_time_flags get_fh_time_flags() const { return m_fh_time_flags; }

private:
  using reserved = std::uint8_t;

private:
  fh_time_ns m_fh_time_ns;
  fh_tz_offset_min m_fh_tz_offset_min;
  fh_dst_offset_min m_fh_dst_offset_min;
  fh_time_flags m_fh_time_flags;
  reserved m_reserved_1;
  reserved m_reserved_2;
  reserved m_reserved_3;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::FH> &p_obj) {

  p_out << "{hd_start_time_ns = " << p_obj.get_fh_time_ns()
        << ", fh_tz_offset_min = " << p_obj.get_fh_tz_offset_min()
        << ", fh_dst_offset_min = " << p_obj.get_fh_dst_offset_min()
        << ", fh_time_flags = "
        << static_cast<uint16_t>(p_obj.get_fh_time_flags()) << '}';

  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
