/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_PAIR_PAIR_OUTPUT_H
#define TNCT_PAIR_PAIR_OUTPUT_H

#include <iostream>
#include <sstream>

#include "tnct/pair/is_pair.h"
#include "tnct/traits/pair_like.h"

namespace tnct::pair {

template <typename t_ouput_stream, traits::pair_like t_pair>
t_ouput_stream &pair2out(t_ouput_stream &p_out, const t_pair &p_pair,
                         const char p_left_delimeter = '(',
                         const char p_right_delimeter = ')') {
  p_out << p_left_delimeter;
  if constexpr (pair::is_pair_v<std::decay<typename t_pair::first_type>>) {
    pair2out(p_out, p_pair.first);
  } else {
    p_out << p_pair.first;
  }
  p_out << ',';
  if constexpr (pair::is_pair_v<std::decay<typename t_pair::second_type>>) {
    tuple2out(p_out, p_pair.second);
  } else {
    p_out << p_pair.second;
  }
  p_out << p_right_delimeter;
  return p_out;
}

} // namespace tnct::pair

template <tnct::traits::pair_like t_pair>
std::ostream &operator<<(std::ostream &p_out, const t_pair &p_pair) {
  // p_out << '(' << p_pair.first << ',' << p_pair.second << ')';
  // return p_out;
  return tnct::pair::pair2out(p_out, p_pair);
}

template <tnct::traits::pair_like t_pair>
std::ostringstream &operator<<(std::ostringstream &p_out,
                               const t_pair &p_pair) {
  // p_out << '(' << p_pair.first << ',' << p_pair.second << ')';
  // return p_out;
  return tnct::pair::pair2out(p_out, p_pair);
}

#endif
