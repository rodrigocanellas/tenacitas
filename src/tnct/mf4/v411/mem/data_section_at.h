/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_AT_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_AT_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::AT> final {
  using at_flags = std::uint16_t;
  using at_creator_index = std::uint16_t;
  using at_md5_checksum = std::uint8_t;
  using at_original_size = std::uint64_t;
  using at_embedded_size = std::uint64_t;
  using at_embedded_data = std::uint8_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(), m_at_flags,
        m_at_creator_index, m_reserved, m_at_md5_checksum, m_at_original_size,
        m_at_embedded_size, m_at_embedded_data);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr at_flags get_at_flags() const { return m_at_flags; }

  constexpr at_creator_index get_at_creator_index() const {
    return m_at_creator_index;
  }

  constexpr at_md5_checksum get_at_md5_checksum() const {
    return m_at_md5_checksum;
  }

  constexpr at_original_size get_at_original_size() const {
    return m_at_original_size;
  }

  constexpr at_embedded_size get_at_embedded_size() const {
    return m_at_embedded_size;
  }

  constexpr at_embedded_data get_at_embedded_data() const {
    return m_at_embedded_data;
  }

private:
  using reserved = std::uint64_t;

private:
  at_flags m_at_flags;
  at_creator_index m_at_creator_index;
  reserved m_reserved;
  at_md5_checksum m_at_md5_checksum;
  at_original_size m_at_original_size;
  at_embedded_size m_at_embedded_size;
  at_embedded_data m_at_embedded_data;
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
