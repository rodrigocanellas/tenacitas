#ifndef TENACITAS_CROSSWORDS_ASY_NEW_WORD_INTO_THE_GRID_H
#define TENACITAS_CROSSWORDS_ASY_NEW_WORD_INTO_THE_GRID_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>
#include <memory>

#include <tenacitas.crosswords/src/typ/explanation.h>
#include <tenacitas.crosswords/src/typ/grid.h>
#include <tenacitas.crosswords/src/typ/orientation.h>
#include <tenacitas.crosswords/src/typ/word.h>

namespace tenacitas::crosswords::asy {

struct new_word_into_the_grid {
  explicit new_word_into_the_grid(const typ::word &p_word,
                                  const typ::explanation &p_explanation)
      : word(p_word), explanation(p_explanation) {}

  new_word_into_the_grid(const new_word_into_the_grid &) = default;
  new_word_into_the_grid(new_word_into_the_grid &&) = default;
  new_word_into_the_grid &operator=(const new_word_into_the_grid &) = default;
  new_word_into_the_grid &operator=(new_word_into_the_grid &&) = default;

  typ::word word;
  typ::explanation explanation;
};

} // namespace tenacitas::crosswords::asy

#endif
