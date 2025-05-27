/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_HD_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_HD_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::HD> final {
  data_section_t() = default;

  data_section_t(std::uint64_t p_hd_start_time_ns,
                 std::int16_t p_hd_tz_offset_min,
                 std::int16_t p_hd_dst_offset_min, std::uint8_t p_hd_time_flags,
                 std::uint8_t p_hd_time_class, std::uint8_t p_hd_flags,
                 double p_hd_start_angle_rad, double p_hd_start_distance_m)
      : m_hd_start_time_ns(p_hd_start_time_ns),
        m_hd_tz_offset_min(p_hd_tz_offset_min),
        m_hd_dst_offset_min(p_hd_dst_offset_min),
        m_hd_time_flags(p_hd_time_flags), m_hd_time_class(p_hd_time_class),
        m_hd_flags(p_hd_flags), m_hd_start_angle_rad(p_hd_start_angle_rad),
        m_hd_start_distance_m(p_hd_start_distance_m) {}

  data_section_t(const std::uint8_t *p_buf, std::size_t) {
    std::uint64_t m_hd_start_time_ns =
        byte_array::from_little<decltype(m_hd_start_time_ns)>(p_buf);
    p_buf += sizeof(m_hd_start_time_ns);

    std::int16_t m_hd_tz_offset_min =
        byte_array::from_little<decltype(m_hd_start_time_ns)>(p_buf);
    p_buf += sizeof(m_hd_tz_offset_min);

    std::int16_t m_hd_dst_offset_min =
        byte_array::from_little<decltype(m_hd_dst_offset_min)>(p_buf);
    p_buf += sizeof(m_hd_dst_offset_min);

    std::uint8_t m_hd_time_flags =
        byte_array::from_little<decltype(m_hd_time_flags)>(p_buf);
    p_buf += sizeof(m_hd_time_flags);

    std::uint8_t m_hd_time_class =
        byte_array::from_little<decltype(m_hd_time_class)>(p_buf);
    p_buf += sizeof(m_hd_time_class);

    std::uint8_t m_hd_flags =
        byte_array::from_little<decltype(m_hd_flags)>(p_buf);
    p_buf += sizeof(m_hd_flags) + sizeof(std::uint8_t);

    double m_hd_start_angle_rad =
        byte_array::from_little<decltype(m_hd_start_angle_rad)>(p_buf);
    p_buf += sizeof(m_hd_start_angle_rad);

    double m_hd_start_distance_m =
        byte_array::from_little<decltype(m_hd_start_distance_m)>(p_buf);
  }

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint64_t get_hd_start_time_ns() const { return m_hd_start_time_ns; }
  std::int16_t get_hd_tz_offset_min() const { return m_hd_tz_offset_min; }
  std::int16_t get_hd_dst_offset_min() const { return m_hd_dst_offset_min; }
  std::uint8_t get_hd_time_flags() const { return m_hd_time_flags; }
  std::uint8_t get_hd_time_class() const { return m_hd_time_class; }
  std::uint8_t get_hd_flags() const { return m_hd_flags; };
  double get_hd_start_angle_rad() const { return m_hd_start_angle_rad; }
  double get_hd_start_distance_m() const { return m_hd_start_distance_m; }

private:
  std::uint64_t m_hd_start_time_ns;
  std::int16_t m_hd_tz_offset_min;
  std::int16_t m_hd_dst_offset_min;
  std::uint8_t m_hd_time_flags;
  std::uint8_t m_hd_time_class;
  std::uint8_t m_hd_flags;
  double m_hd_start_angle_rad;
  double m_hd_start_distance_m;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::HD> &p_obj) {
  p_out << '{' << p_obj.get_hd_start_time_ns() << " "
        << p_obj.get_hd_tz_offset_min() << ' ' << p_obj.get_hd_dst_offset_min()
        << ' ' << static_cast<uint16_t>(p_obj.get_hd_time_flags()) << ' '
        << static_cast<uint16_t>(p_obj.get_hd_time_class()) << ' '
        << static_cast<uint16_t>(p_obj.get_hd_flags()) << ' '
        << p_obj.get_hd_start_angle_rad() << ' '
        << p_obj.get_hd_start_distance_m() << '}';
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
