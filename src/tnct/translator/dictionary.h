/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRANSLATOR_DICTIONARY_H
#define TNCT_TRANSLATOR_DICTIONARY_H

#include <iostream>
#include <map>
#include <string>

namespace tnct::translator {

template <std::unsigned_integral t_index = uint32_t>
using dictionary = std::map<t_index, std::string>;

template <std::unsigned_integral t_index = uint32_t>
inline std::ostream &operator<<(std::ostream &p_out,
                                const dictionary<t_index> &p_dictionary) {

  for (const typename dictionary<t_index>::value_type &p_value : p_dictionary) {
    p_out << '(' << p_value.first << " - " << p_value.second << ") ";
  }
  return p_out;
}

} // namespace tnct::translator

#endif
