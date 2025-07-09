/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CN_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CN_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CN> final {
  using cn_type = std::uint8_t;
  using cn_sync_type = std::uint8_t;
  using cn_data_type = std::uint8_t;
  using cn_bit_offset = std::uint8_t;
  using cn_byte_offset = std::uint32_t;
  using cn_bit_count = std::uint32_t;
  using cn_flags = std::uint32_t;
  using cn_inval_bit_pos = std::uint32_t;
  using cn_precision = std::uint8_t;
  using cn_attachment_count = std::uint16_t;
  using cn_val_range_min = double;
  using cn_val_range_max = double;
  using cn_limit_min = double;
  using cn_limit_max = double;
  using cn_limit_ext_min = double;
  using cn_limit_ext_max = double;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(), m_cn_type,
        m_cn_sync_type, m_cn_data_type, m_cn_bit_offset, m_cn_byte_offset,
        m_cn_bit_count, m_cn_flags, m_cn_inval_bit_pos, m_cn_precision,
        m_reserved, m_cn_attachment_count, m_cn_val_range_min,
        m_cn_val_range_max, m_cn_limit_min, m_cn_limit_max, m_cn_limit_ext_min,
        m_cn_limit_ext_max);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr cn_type get_cn_type() const { return m_cn_type; }

  constexpr cn_sync_type get_cn_sync_type() const { return m_cn_sync_type; }

  constexpr cn_data_type get_cn_data_type() const { return m_cn_data_type; }

  constexpr cn_bit_offset get_cn_bit_offset() const { return m_cn_bit_offset; }

  constexpr cn_byte_offset get_cn_byte_offset() const {
    return m_cn_byte_offset;
  }

  constexpr cn_bit_count get_cn_bit_count() const { return m_cn_bit_count; }

  constexpr cn_flags get_cn_flags() const { return m_cn_flags; }

  constexpr cn_inval_bit_pos get_cn_inval_bit_pos() const {
    return m_cn_inval_bit_pos;
  }

  constexpr cn_precision get_cn_precision() const { return m_cn_precision; }

  constexpr cn_attachment_count get_cn_attachment_count() const {
    return m_cn_attachment_count;
  }

  constexpr cn_val_range_min get_cn_val_range_min() const {
    return m_cn_val_range_min;
  }

  constexpr cn_val_range_max get_cn_val_range_max() const {
    return m_cn_val_range_max;
  }

  constexpr cn_limit_min get_cn_limit_min() const { return m_cn_limit_min; }

  constexpr cn_limit_max get_cn_limit_max() const { return m_cn_limit_max; };

  constexpr cn_limit_ext_min get_cn_limit_ext_min() const {
    return m_cn_limit_ext_min;
  }

  constexpr cn_limit_ext_max get_cn_limit_ext_max() const {
    return m_cn_limit_ext_max;
  }

private:
  using reserved = std::uint8_t;

private:
  cn_type m_cn_type;
  cn_sync_type m_cn_sync_type;
  cn_data_type m_cn_data_type;
  cn_bit_offset m_cn_bit_offset;
  cn_byte_offset m_cn_byte_offset;
  cn_bit_count m_cn_bit_count;
  cn_flags m_cn_flags;
  cn_inval_bit_pos m_cn_inval_bit_pos;
  cn_precision m_cn_precision;
  reserved m_reserved;
  cn_attachment_count m_cn_attachment_count;
  cn_val_range_min m_cn_val_range_min;
  cn_val_range_max m_cn_val_range_max;
  cn_limit_min m_cn_limit_min;
  cn_limit_max m_cn_limit_max;
  cn_limit_ext_min m_cn_limit_ext_min;
  cn_limit_ext_max m_cn_limit_ext_max;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CN> &p_obj) {
  p_out << "{"
        << "cn_type = " << static_cast<std::uint16_t>(p_obj.get_cn_type())
        << ", cn_sync_type = "
        << static_cast<std::uint16_t>(p_obj.get_cn_sync_type())
        << ", cn_data_type = "
        << static_cast<std::uint16_t>(p_obj.get_cn_data_type())
        << ", cn_bit_offset  = "
        << static_cast<std::uint16_t>(p_obj.get_cn_bit_offset())
        << ", cn_byte_offset  = " << p_obj.get_cn_byte_offset()
        << ", cn_bit_count = " << p_obj.get_cn_bit_count()
        << ", cn_flags = " << p_obj.get_cn_flags()
        << ", cn_inval_bit_pos = " << p_obj.get_cn_inval_bit_pos()
        << ", cn_precision = "
        << static_cast<std::uint16_t>(p_obj.get_cn_precision())
        << ", cn_attachment_count = " << p_obj.get_cn_attachment_count()
        << ", cn_val_range_min = " << p_obj.get_cn_val_range_min()
        << ", cn_val_range_max = " << p_obj.get_cn_val_range_max()
        << ", cn_limit_min  = " << p_obj.get_cn_limit_min()
        << ", cn_limit_max = " << p_obj.get_cn_limit_max()
        << ", cn_limit_ext_min = " << p_obj.get_cn_limit_ext_min()
        << ", cn_limit_ext_max = " << p_obj.get_cn_limit_ext_max()

        << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
