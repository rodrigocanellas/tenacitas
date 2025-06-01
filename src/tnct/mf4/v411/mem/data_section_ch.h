/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CH_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CH_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CH> final {

  using ch_element_count = std::uint32_t;
  using ch_type = std::uint8_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(),
        m_ch_element_count, m_ch_type, m_reserved_1, m_reserved_2,
        m_reserved_3);
  }

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint32_t get_ch_element_count() const { return m_ch_element_count; }
  std::uint8_t get_ch_type() const { return m_ch_type; }

private:
  using reserved = std::uint8_t;

private:
  ch_element_count m_ch_element_count;
  ch_type m_ch_type;
  reserved m_reserved_1;
  reserved m_reserved_2;
  reserved m_reserved_3;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CH> &p_obj) {

  p_out << "{ch_element_count = " << p_obj.get_ch_element_count()
        << ", ch_type = " << static_cast<uint16_t>(p_obj.get_ch_type()) << '}';

  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
