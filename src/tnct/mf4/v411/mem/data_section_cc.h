/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CC_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CC_H

#include <iostream>
#include <vector>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CC> final {
  using cc_type = std::uint8_t;
  using cc_precision = std::uint8_t;
  using cc_flags = std::uint16_t;
  using cc_ref_count = std::uint16_t;
  using cc_val_count = std::uint16_t;
  using cc_phy_range_min = double;
  using cc_phy_range_max = double;
  using cc_val = double;

  using cc_vals = std::vector<cc_val>;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    constexpr std::size_t _sizeof{
        sizeof(cc_type) + sizeof(cc_precision) + sizeof(cc_flags) +
        sizeof(cc_ref_count) + sizeof(cc_val_count) + sizeof(cc_phy_range_min) +
        sizeof(cc_phy_range_max)};

    byte_array::from_buffer_little_endian(
        p_logger, p_buf, _sizeof, std::source_location::current(), m_cc_type,
        m_cc_precision, m_cc_flags, m_cc_ref_count, m_cc_val_count,
        m_cc_phy_range_min, m_cc_phy_range_max);

    if (m_cc_val_count != 0) {
      m_cc_vals = cc_vals(p_size - _sizeof, cc_val{0.0});

      for (cc_val_count _i = 0; _i < m_cc_val_count; ++_i) {
        m_cc_vals[_i] = byte_array::from_little<cc_val>(p_buf + p_size +
                                                        (_i * sizeof(cc_val)));
      }
    }
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr cc_type get_cc_type() const { return m_cc_type; }

  constexpr cc_precision get_cc_precision() const { return m_cc_precision; }

  constexpr cc_flags get_cc_flags() const { return m_cc_flags; }

  constexpr cc_ref_count get_cc_ref_count() const { return m_cc_ref_count; }

  constexpr cc_val_count get_cc_val_count() const { return m_cc_val_count; }

  constexpr cc_phy_range_min get_cc_phy_range_min() const {
    return m_cc_phy_range_min;
  }

  constexpr cc_phy_range_max get_cc_phy_range_max() const {
    return m_cc_phy_range_max;
  };

  const cc_vals &get_cc_vals() const { return m_cc_vals; }

private:
  cc_type m_cc_type;
  cc_precision m_cc_precision;
  cc_flags m_cc_flags;
  cc_ref_count m_cc_ref_count;
  cc_val_count m_cc_val_count;
  cc_phy_range_min m_cc_phy_range_min;
  cc_phy_range_max m_cc_phy_range_max;
  cc_vals m_cc_vals;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CC> &p_obj) {
  p_out << "{"
        << "cc_type = " << static_cast<std::uint16_t>(p_obj.get_cc_type())
        << ", cc_precision = "
        << static_cast<std::uint16_t>(p_obj.get_cc_precision())
        << ", cc_flags = " << p_obj.get_cc_flags()
        << ", cc_ref_count = " << p_obj.get_cc_ref_count()
        << ", cc_val_count = " << p_obj.get_cc_val_count()
        << ", cc_phy_range_min  = " << p_obj.get_cc_phy_range_min()
        << ", cc_phy_range_max = " << p_obj.get_cc_phy_range_max()
        << ", cc_vals amount = " << p_obj.get_cc_vals().size()

        << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
