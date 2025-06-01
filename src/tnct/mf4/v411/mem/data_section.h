/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_H

#include <iostream>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <block_id t_block_Id> struct data_section_t final {
  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &, const std::uint8_t *, std::size_t) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <block_id t_block_Id>
std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<t_block_Id> &) {
  p_out << "{}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
