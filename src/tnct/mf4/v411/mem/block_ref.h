/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_BLOCK_REF_H
#define TNCT_MF4_V411_MEM_BLOCK_REF_H

#include <iostream>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/block_index.h"

namespace tnct::mf4::v411::mem {

struct block_ref {
  block_ref() = default;

  block_ref(block_id p_block_id, block_index p_block_index)
      : m_block_id(p_block_id), m_block_index(p_block_index) {}

  block_ref(const block_ref &) = default;

  block_ref(block_ref &&) = default;

  ~block_ref() = default;

  block_ref &operator=(const block_ref &) = default;

  block_ref &operator=(block_ref &&) = default;

  constexpr bool operator==(const block_ref &p_obj) const {
    return (m_block_id == p_obj.m_block_id) &&
           (m_block_index == p_obj.m_block_index);
  }

  constexpr bool operator!=(const block_ref &p_obj) const {
    return !(*this == p_obj);
  }

  constexpr bool operator<(const block_ref &p_obj) const {
    if (m_block_id < p_obj.m_block_id) {
      return true;
    }
    if (m_block_id > p_obj.m_block_id) {
      return false;
    }

    if (m_block_index < p_obj.m_block_index) {
      return true;
    }

    return false;
  }

  block_id get_id() const { return m_block_id; }

  block_index get_index() const { return m_block_index; }

private:
  block_id m_block_id{block_id::HD};
  block_index m_block_index;
};

std::ostream &operator<<(std::ostream &p_out, const block_ref &p_obj) {
  p_out << '{' << block_id_converter::to_str(p_obj.get_id()) << ", "
        << p_obj.get_index() << '}';
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
