/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CG_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CG_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CG> final {

  using cg_record_id = std::uint64_t;
  using cg_cycle_count = std::uint64_t;
  using cg_flags = std::uint16_t;
  using cg_path_separator = std::uint16_t;
  using cg_data_bytes = std::uint32_t;
  using cg_inval_bytes = std::uint32_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(),
        m_cg_record_id, m_cg_cycle_count, m_cg_flags, m_cg_path_separator,
        m_reserved, m_cg_data_bytes, m_cg_inval_bytes);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr cg_record_id get_cg_record_id() const { return m_cg_record_id; }

  constexpr cg_cycle_count get_cg_cycle_count() const {
    return m_cg_cycle_count;
  }

  constexpr cg_flags get_cg_flags() const { return m_cg_flags; }

  constexpr cg_path_separator get_cg_path_separator() const {
    return m_cg_path_separator;
  }

  constexpr cg_data_bytes get_cg_data_bytes() const { return m_cg_data_bytes; }

  constexpr cg_inval_bytes get_cg_inval_bytes() const {
    return m_cg_inval_bytes;
  }

private:
  using reserved = std::uint32_t;

private:
  cg_record_id m_cg_record_id;
  cg_cycle_count m_cg_cycle_count;
  cg_flags m_cg_flags;
  cg_path_separator m_cg_path_separator;
  reserved m_reserved;
  cg_data_bytes m_cg_data_bytes;
  cg_inval_bytes m_cg_inval_bytes;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CG> &p_obj) {
  p_out << "{"
        << "cg_record_id = " << p_obj.get_cg_record_id()
        << ", cg_cycle_count = " << p_obj.get_cg_cycle_count()
        << ", cg_flags = " << p_obj.get_cg_flags()
        << ", cg_path_separator  = " << p_obj.get_cg_path_separator()
        << ", cg_data_bytes = " << p_obj.get_cg_data_bytes()
        << ", cg_inval_bytes = " << p_obj.get_cg_inval_bytes() << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
