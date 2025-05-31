/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_CH_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_CH_H

#include <iostream>

#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::CH> final {
  data_section_t() = default;

  data_section_t(std::uint32_t p_ch_element_count, std::uint8_t p_ch_type)
      : m_ch_element_count(p_ch_element_count), m_ch_type(p_ch_type) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint32_t get_ch_element_count() const { return m_ch_element_count; }
  std::uint8_t get_ch_type() const { return m_ch_type; }

private:
  std::uint32_t m_ch_element_count;
  std::uint8_t m_ch_type;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::CH> &p_obj) {

  p_out << "{ch_element_count = " << p_obj.get_ch_element_count()
        << ", ch_type = " << static_cast<uint16_t>(p_obj.get_ch_type()) << '}';

  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
