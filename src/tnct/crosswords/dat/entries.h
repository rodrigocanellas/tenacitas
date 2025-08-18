#ifndef TNCT_CROSSWORDS_DAT_ENTRIES_H
#define TNCT_CROSSWORDS_DAT_ENTRIES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <vector>

#include "tnct/crosswords/dat/entry.h"
#include "tnct/crosswords/dat/explanation.h"
#include "tnct/crosswords/dat/word.h"

namespace tnct::crosswords::dat {

/// \brief A set of \p entry
struct entries {

  using size = uint8_t;
  using collection = std::vector<entry>;
  using const_entry_ite = collection::const_iterator;
  using entry_ite = collection::iterator;

  entries() = default;
  entries(const entries &) = default;
  entries(entries &&) = default;
  inline entries(std::initializer_list<entry> &&p_enties) {
    m_collection = std::move(p_enties);
  }

  ~entries() = default;

  entries &operator=(const entries &) = default;
  entries &operator=(entries &&) = default;

  inline void add_entry(word &&p_word, explanation &&p_explanation) {
    m_collection.push_back({std::move(p_word), std::move(p_explanation)});
  }

  inline void add_entry(entry &&p_entry) {
    m_collection.push_back(std::move(p_entry));
  }

  inline const_entry_ite begin() const { return m_collection.begin(); }
  inline const_entry_ite end() const { return m_collection.end(); }

  inline entry_ite begin() { return m_collection.begin(); }
  inline entry_ite end() { return m_collection.end(); }

  inline size get_num_entries() const {
    return static_cast<size>(m_collection.size());
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const entries &p_entries) {
    p_out << "\nENTRIES\n";
    for (const entry &_entry : p_entries) {
      p_out << _entry.get_word() << '\t' << _entry.get_explanation() << '\n';
    }
    return p_out;
  }

  void clear() { m_collection.clear(); }

  bool empty() const { return m_collection.empty(); }

private:
  collection m_collection;
};

} // namespace tnct::crosswords::dat

#endif
