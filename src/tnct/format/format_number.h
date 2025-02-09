/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_FORMAT_FORMAT_NUMBER_H
#define TNCT_FORMAT_FORMAT_NUMBER_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "tnct/format/align.h"
#include "tnct/format/max_str_length.h"

namespace tnct::format {

/// \brief Formats a number with the maximum characters necessary to represent
/// the number to string, filled with \p p_fill and aligned with \p p_align
///
/// \tparam t_num_type is the type of the number to be formated
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
requires(std::integral<t_num_type> || std::floating_point<t_num_type>)

    inline std::string format_max_number(t_num_type p_num, char p_fill = '0',
                                         align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill) << std::setw(max_str_length<t_num_type>())
          << (std::is_same_v<t_num_type, uint8_t>
                  ? static_cast<uint16_t>(p_num)
                  : (std::is_same_v<int8_t, t_num_type>
                         ? static_cast<int16_t>(p_num)
                         : p_num));
  return _stream.str();
}

template <typename t_num_type>
requires(std::integral<t_num_type> || std::floating_point<t_num_type>)

    inline std::string
    format_fix_number(t_num_type p_num, uint8_t p_width, char p_fill = '0',
                      align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill) << std::setw(p_width)
          << (std::is_same_v<t_num_type, uint8_t>
                  ? static_cast<uint16_t>(p_num)
                  : (std::is_same_v<int8_t, t_num_type>
                         ? static_cast<int16_t>(p_num)
                         : p_num));
  return _stream.str();
}

} // namespace tnct::format

#endif
