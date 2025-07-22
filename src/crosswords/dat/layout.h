#ifndef TENACITAS_SRC_CROSSWORDS_DAT_LAYOUT_H
#define TENACITAS_SRC_CROSSWORDS_DAT_LAYOUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/crosswords/dat/entries.h"
#include "tenacitas/src/crosswords/dat/entry.h"
#include "tenacitas/src/crosswords/dat/explanation.h"
#include "tenacitas/src/crosswords/dat/index.h"
#include "tenacitas/src/crosswords/dat/orientation.h"
#include "tenacitas/src/crosswords/dat/word.h"

namespace tenacitas::src::crosswords::dat {

/// \brief An \p entry with a \p orientation and \p coordinate defined
struct layout {
  layout() = default;

  layout(entries::const_entry_ite p_entry, uint16_t p_id = 0)
      : m_entry(p_entry), m_id(p_id) {}

  layout(const layout &) = default;
  layout(layout &&) = default;

  ~layout() = default;

  layout &operator=(const layout &) = default;
  layout &operator=(layout &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const layout &p_layout) {
    p_out << "('" << p_layout.m_entry->get_word() << "',"
          << static_cast<uint16_t>(p_layout.m_row) << ','
          << static_cast<uint16_t>(p_layout.m_col) << ','
          << p_layout.m_orientation << char(')');

    return p_out;
  }

  inline const word &get_word() const { return m_entry->get_word(); }
  inline const explanation &get_explanation() const {
    return m_entry->get_explanation();
  }
  inline index get_row() const { return m_row; }
  inline index get_col() const { return m_col; }
  inline orientation get_orientation() const { return m_orientation; }
  inline void set_entry(entries::const_entry_ite p_entry) { m_entry = p_entry; }
  inline void set_row(index p_x) { m_row = p_x; }
  inline void set_col(index p_y) { m_col = p_y; }
  inline void set_orientation(orientation p_orientation) {
    m_orientation = p_orientation;
  }
  inline bool is_positioned() const {
    return m_orientation != orientation::undef;
  }
  uint16_t get_id() const { return m_id; }

  bool get_id(index p_row, index p_col) const {
    if ((m_row == p_row) && (m_col == p_col)) {
      return true;
    }
    //    if (m_orientation == orientation::hori) {
    //      if ((m_row == p_row) && (p_col >= m_col) &&
    //          (p_col < (m_col + m_entry->get_word().size()))) {
    //        return {m_id};
    //      }
    //    } else {
    //      if ((m_col == p_col) && (p_row >= m_row) &&
    //          (p_row < (m_row + m_entry->get_word().size()))) {
    //        return {m_id};
    //      }
    //    }
    return false;
  }

  void reset() {
    m_row = max_row;
    m_col = max_col;
    m_orientation = orientation::undef;
  }

private:
  entries::const_entry_ite m_entry;
  uint16_t m_id;
  index m_row{max_row};
  index m_col{max_col};
  orientation m_orientation{orientation::undef};
};

} // namespace tenacitas::src::crosswords::dat

#endif
