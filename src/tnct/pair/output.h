/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_PAIR_OUTPUT_H
#define TNCT_PAIR_OUTPUT_H

#include <iostream>
#include <utility>

#include "tnct/pair/cpt/is_pair.h"

template <typename t_char, typename t_traits, tnct::pair::cpt::is_pair t_pair>
std::basic_ostream<t_char, t_traits> &
operator<<(std::basic_ostream<t_char, t_traits> &p_out, const t_pair &p_pair)
{
  p_out << '(' << p_pair.first << ',' << p_pair.second << ')';
  return p_out;
}

template <typename t_char, typename t_traits, typename t_first,
          typename t_second>
std::basic_ostream<t_char, t_traits> &
operator<<(std::basic_ostream<t_char, t_traits> &p_out,
           const std::pair<t_first, t_second>   &p_pair)
{
  p_out << '(' << p_pair.first << ',' << p_pair.second << ')';
  return p_out;
}

#endif
