/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_DG_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_DG_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::DG> final {

  using dg_rec_id_size = std::uint8_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(),
        m_dg_rec_id_size, m_reserved_1, m_reserved_2, m_reserved_3,
        m_reserved_4, m_reserved_5, m_reserved_6, m_reserved_7);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr dg_rec_id_size get_dg_rec_id_size() const {
    return m_dg_rec_id_size;
  }

private:
  using reserved = std::uint8_t;

private:
  dg_rec_id_size m_dg_rec_id_size;
  reserved m_reserved_1;
  reserved m_reserved_2;
  reserved m_reserved_3;
  reserved m_reserved_4;
  reserved m_reserved_5;
  reserved m_reserved_6;
  reserved m_reserved_7;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::DG> &p_obj) {

  p_out << "{dg_rec_id_size = "
        << static_cast<uint16_t>(p_obj.get_dg_rec_id_size()) << '}';

  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
