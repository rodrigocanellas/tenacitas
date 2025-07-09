/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CA_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CA_H

#include <iostream>
#include <vector>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CA> final {
  using ca_type = std::uint8_t;
  using ca_storage = std::uint8_t;
  using ca_ndim = std::uint16_t;
  using ca_flags = std::uint32_t;
  using ca_byte_offset_base = std::int32_t;
  using ca_inval_bit_pos_base = std::uint32_t;
  using ca_dim_size = std::uint64_t;
  using ca_axis_value = double;
  using ca_cycle_count = std::uint64_t;

  using ca_ref_count = std::uint16_t;
  using ca_val_count = std::uint16_t;
  using ca_phy_range_min = double;
  using ca_phy_range_max = double;
  using ca_val = double;

  using ca_vals = std::vector<ca_val>;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    constexpr std::size_t _sizeof{
        sizeof(ca_type) + sizeof(ca_storage) + sizeof(ca_flags) +
        sizeof(ca_ref_count) + sizeof(ca_val_count) + sizeof(ca_phy_range_min) +
        sizeof(ca_phy_range_max)};

    byte_array::from_buffer_little_endian(
        p_logger, p_buf, _sizeof, std::source_location::current(), m_ca_type,
        m_ca_storage, m_ca_flags, m_ca_ref_count, m_ca_val_count,
        m_ca_phy_range_min, m_ca_phy_range_max);

    if (m_ca_val_count != 0) {
      m_ca_vals = ca_vals(p_size - _sizeof, ca_val{0.0});

      for (ca_val_count _i = 0; _i < m_ca_val_count; ++_i) {
        m_ca_vals[_i] = byte_array::from_little<ca_val>(p_buf + p_size +
                                                        (_i * sizeof(ca_val)));
      }
    }
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr ca_type get_ca_type() const { return m_ca_type; }

  constexpr ca_storage get_ca_storage() const { return m_ca_storage; }

  constexpr ca_ndim get_ca_ndim() const { return m_ndim; }

  constexpr ca_flags get_ca_flags() const { return m_ca_flags; }

  constexpr ca_byte_offset_base get_ca_byte_offset_base() const {
    return m_ca_byte_offset_base;
  }

  constexpr ca_inval_bit_pos_base get_ca_inval_bit_pos_base() const {
    return m_ca_inval_bit_pos_base;
  }

  constexpr ca_dim_size get_ca_dim_size() const { return m_ca_dim_size; }

  constexpr ca_axis_value get_ca_axis_value() const { return m_ca_axis_value; }

  constexpr ca_cycle_count get_ca_cycle_count() const {
    return m_ca_cycle_count;
  }

private:
  ca_type m_ca_type;
  ca_storage m_ca_storage;
  ca_ndim m_ndim;
  ca_flags m_ca_flags;
  ca_byte_offset_base m_ca_byte_offset_base;
  ca_inval_bit_pos_base m_ca_inval_bit_pos_base;
  ca_dim_size m_ca_dim_size;
  ca_axis_value m_ca_axis_value;
  ca_cycle_count m_ca_cycle_count;

  ca_ref_count m_ca_ref_count;
  ca_val_count m_ca_val_count;
  ca_phy_range_min m_ca_phy_range_min;
  ca_phy_range_max m_ca_phy_range_max;
  ca_vals m_ca_vals;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CA> &p_obj) {
  p_out << "{"
        << "ca_type = " << static_cast<std::uint16_t>(p_obj.get_ca_type())
        << ", ca_storage = "
        << static_cast<std::uint16_t>(p_obj.get_ca_storage())
        << ", ca_ndim= " << p_obj.get_ca_ndim()
        << ", ca_flags = " << p_obj.get_ca_flags()
        << ", ca_byte_offset_base = " << p_obj.get_ca_byte_offset_base()
        << ", ca_inval_bit_pos_base = " << p_obj.get_ca_inval_bit_pos_base()
        << ", ca_dim_size = " << p_obj.get_ca_dim_size()
        << ", ca_axis_value = " << p_obj.get_ca_axis_value()
        << ", ca_cycle_count = " << p_obj.get_ca_cycle_count() << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
