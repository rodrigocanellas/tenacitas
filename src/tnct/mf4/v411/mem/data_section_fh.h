/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_FH_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_FH_H

#include <iostream>

#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::FH> final {
  data_section_t() = default;

  data_section_t(std::uint64_t p_fh_time_ns, std::int16_t p_fh_tz_offset_min,
                 std::int16_t p_fh_dst_offset_min, std::uint8_t p_fh_time_flags)
      : m_fh_time_ns(p_fh_time_ns), m_fh_tz_offset_min(p_fh_tz_offset_min),
        m_fh_dst_offset_min(p_fh_dst_offset_min),
        m_fh_time_flags(p_fh_time_flags) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint64_t get_fh_time_ns() const { return m_fh_time_ns; }
  std::int16_t get_fh_tz_offset_min() const { return m_fh_tz_offset_min; }
  std::int16_t get_fh_dst_offset_min() const { return m_fh_dst_offset_min; }
  std::uint8_t get_fh_time_flags() const { return m_fh_time_flags; }

private:
  std::uint64_t m_fh_time_ns;
  std::int16_t m_fh_tz_offset_min;
  std::int16_t m_fh_dst_offset_min;
  std::uint8_t m_fh_time_flags;
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
