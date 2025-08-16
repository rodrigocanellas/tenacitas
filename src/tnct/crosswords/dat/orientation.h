#ifndef TNCT_CROSSWORDS_DAT_ORIENTATION_H
#define TNCT_CROSSWORDS_DAT_ORIENTATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

namespace tnct::crosswords::dat {

/// \brief Possible orientations of a \p word in a grid
enum class orientation : char { vert = 'V', hori = 'H', undef = 'U' };

inline std::ostream &operator<<(std::ostream &p_out,
                                orientation p_orientation) {
  p_out << static_cast<char>(p_orientation);
  return p_out;
}

} // namespace tnct::crosswords::dat

#endif
