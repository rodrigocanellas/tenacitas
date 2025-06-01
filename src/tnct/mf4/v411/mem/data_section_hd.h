/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_HD_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_HD_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::HD> final {
  using hd_start_time_ns = std::uint64_t;
  using hd_tz_offset_min = std::int16_t;
  using hd_dst_offset_min = std::int16_t;
  using hd_time_flags = std::uint8_t;
  using hd_time_class = std::uint8_t;
  using hd_flags = std::uint8_t;

  using hd_start_angle_rad = double;
  using hd_start_distance_m = double;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(),
        m_hd_start_time_ns, m_hd_tz_offset_min, m_hd_dst_offset_min,
        m_hd_time_flags, m_hd_time_class, m_hd_flags, m_reserved,
        m_hd_start_angle_rad, m_hd_start_distance_m);
  }

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  constexpr hd_start_time_ns get_hd_start_time_ns() const {
    return m_hd_start_time_ns;
  }

  constexpr hd_tz_offset_min get_hd_tz_offset_min() const {
    return m_hd_tz_offset_min;
  }

  constexpr hd_dst_offset_min get_hd_dst_offset_min() const {
    return m_hd_dst_offset_min;
  }

  constexpr hd_time_flags get_hd_time_flags() const { return m_hd_time_flags; }

  constexpr hd_time_class get_hd_time_class() const { return m_hd_time_class; }

  constexpr hd_flags get_hd_flags() const { return m_hd_flags; };

  constexpr hd_start_angle_rad get_hd_start_angle_rad() const {
    return m_hd_start_angle_rad;
  }

  constexpr hd_start_distance_m get_hd_start_distance_m() const {
    return m_hd_start_distance_m;
  }

private:
  using reserved = std::uint8_t;

private:
  hd_start_time_ns m_hd_start_time_ns;
  hd_tz_offset_min m_hd_tz_offset_min;
  hd_dst_offset_min m_hd_dst_offset_min;
  hd_time_flags m_hd_time_flags;
  hd_time_class m_hd_time_class;
  hd_flags m_hd_flags;
  reserved m_reserved;
  hd_start_angle_rad m_hd_start_angle_rad;
  hd_start_distance_m m_hd_start_distance_m;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::HD> &p_obj) {
  p_out << "{hd_start_time_ns = " << p_obj.get_hd_start_time_ns()
        << ", hd_tz_offset_min = " << p_obj.get_hd_tz_offset_min()
        << ", hd_dst_offset_min = " << p_obj.get_hd_dst_offset_min()
        << ", hd_time_flags = "
        << static_cast<uint16_t>(p_obj.get_hd_time_flags())
        << ", hd_time_class = "
        << static_cast<uint16_t>(p_obj.get_hd_time_class())
        << ", hd_flags = " << static_cast<uint16_t>(p_obj.get_hd_flags())
        << ", hd_start_angle_rad = " << p_obj.get_hd_start_angle_rad()
        << ", hd_start_distance_m = " << p_obj.get_hd_start_distance_m() << '}';
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
