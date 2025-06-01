/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_TX_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_TX_H

#include <iostream>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/data_section.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::TX> final {
  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger & /*p_logger*/, const std::uint8_t *p_buf,
                 std::size_t p_size)
      : m_text(reinterpret_cast<const char *>(p_buf), p_size - 1) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  const std::string &get_text() const { return m_text; }

private:
  std::string m_text;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::TX> &p_obj) {
  p_out << '\'' << p_obj.get_text() << '\'';
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
