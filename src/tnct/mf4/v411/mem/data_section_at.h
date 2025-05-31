/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_AT_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_AT_H

#include <iostream>

#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::AT> final {
  data_section_t() = default;

  data_section_t(std::uint16_t p_at_flags, std::uint16_t p_at_creator_index,
                 std::uint8_t p_at_md5_checksum,
                 std::uint64_t p_at_original_size,
                 std::uint64_t p_at_embedded_size,
                 std::uint8_t p_at_embedded_data)
      : m_at_flags(p_at_flags), m_at_creator_index(p_at_creator_index),
        m_at_md5_checksum(p_at_md5_checksum),
        m_at_original_size(p_at_original_size),
        m_at_embedded_size(p_at_embedded_size),
        m_at_embedded_data(p_at_embedded_data) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint16_t get_at_flags() const { return m_at_flags; }
  std::uint16_t get_at_creator_index() const { return m_at_creator_index; }
  std::uint8_t get_at_md5_checksum() const { return m_at_md5_checksum; }
  std::uint64_t get_at_original_size() const { return m_at_original_size; }
  std::uint64_t get_at_embedded_size() const { return m_at_embedded_size; }
  std::uint8_t get_at_embedded_data() const { return m_at_embedded_data; }

private:
  std::uint16_t m_at_flags;
  std::uint16_t m_at_creator_index;
  std::uint8_t m_at_md5_checksum;
  std::uint64_t m_at_original_size;
  std::uint64_t m_at_embedded_size;
  std::uint8_t m_at_embedded_data;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::AT> &p_obj) {
  p_out << "{ at_flags = " << p_obj.get_at_flags()
        << ", at_creator_index = " << p_obj.get_at_creator_index()
        << ", at_md5_checksum = "
        << static_cast<std::uint16_t>(p_obj.get_at_md5_checksum())
        << ", at_original_size = " << p_obj.get_at_original_size()
        << ", at_embedded_size = " << p_obj.get_at_embedded_size()
        << ", at_embedded_data = "
        << static_cast<std::uint16_t>(p_obj.get_at_embedded_data()) << "}";

  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
