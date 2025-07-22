#ifndef TENACITAS_SRC_CROSSWORDS_DAT_ENTRY_H
#define TENACITAS_SRC_CROSSWORDS_DAT_ENTRY_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/crosswords/dat/explanation.h"
#include "tenacitas/src/crosswords/dat/word.h"

namespace tenacitas::src::crosswords::dat {

/// \brief A \p word and its \p explanation
struct entry {
  entry() = delete;

  entry(const entry &) = default;

  entry(entry &&) = default;

  entry(word &&p_word, explanation &&p_explanation)
      : m_word(std::move(p_word)), m_explanation(std::move(p_explanation)) {}

  ~entry() = default;

  entry &operator=(const entry &) = default;
  entry &operator=(entry &&) = default;

  inline const word &get_word() const { return m_word; }

  inline const explanation &get_explanation() const { return m_explanation; }

private:
  word m_word;
  explanation m_explanation;
};

} // namespace tenacitas::src::crosswords::dat

#endif
