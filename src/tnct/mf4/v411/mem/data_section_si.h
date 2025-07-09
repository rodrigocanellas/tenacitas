/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_SI_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_SI_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::SI> final {

  using si_type = std::uint8_t;
  using si_bus_type = std::uint8_t;
  using si_flags = std::uint8_t;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(), m_si_type,
        m_si_bus_type, m_si_flags, m_reserved_1, m_reserved_2, m_reserved_3,
        m_reserved_4, m_reserved_5);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  constexpr si_type get_si_type() const { return m_si_type; }
  constexpr si_bus_type get_si_bus_type() const { return m_si_bus_type; }
  constexpr si_flags get_si_flags() const { return m_si_flags; }

private:
  using reserved = std::uint8_t;

private:
  si_type m_si_type;
  si_bus_type m_si_bus_type;
  si_flags m_si_flags;
  reserved m_reserved_1;
  reserved m_reserved_2;
  reserved m_reserved_3;
  reserved m_reserved_4;
  reserved m_reserved_5;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::SI> &p_obj) {
  p_out << "{"
        << "si_type = " << static_cast<std::uint16_t>(p_obj.get_si_type())
        << ", si_bus_type = "
        << static_cast<std::uint16_t>(p_obj.get_si_bus_type())
        << ", si_flags = " << static_cast<std::uint16_t>(p_obj.get_si_flags())
        << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
