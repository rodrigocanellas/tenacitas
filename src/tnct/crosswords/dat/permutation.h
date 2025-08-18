#ifndef TNCT_CROSSWORDS_DAT_PERMUTATION_H
#define TNCT_CROSSWORDS_DAT_PERMUTATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <vector>

#include "tnct/crosswords/dat/entries.h"

namespace tnct::crosswords::dat {

/// \brief A combination of entries
using permutation = std::vector<entries::const_entry_ite>;

inline std::ostream &operator<<(std::ostream &p_out,
                                const permutation &p_permutation) {
  p_out << "{ ";
  for (entries::const_entry_ite _entry : p_permutation) {
    p_out << _entry->get_word() << ' ';
  }
  p_out << '}';
  return p_out;
}

} // namespace tnct::crosswords::dat

#endif
