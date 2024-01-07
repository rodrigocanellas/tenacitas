/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ALG_FORMAT_NUMBER_H
#define TENACITAS_LIB_ALG_FORMAT_NUMBER_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <tenacitas.lib/src/alg/internal/max_str_length.h>
#include <tenacitas.lib/src/dat/align.h>

namespace tenacitas::lib::alg {

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
inline std::string format_number(t_num_type p_num, char p_fill = '0',
                                   tld::align p_align = tld::align::right) {
  std::stringstream _stream;
  _stream << (p_align == tld::align::right ? std::right : std::left)
          << std::fixed << std::setfill(p_fill)
          << std::setw(tla::internal::max_str_length<t_num_type>()) << p_num;
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string format_number(uint8_t p_num, char p_fill = '0',
                                 tld::align p_align = tld::align::right) {
  std::stringstream _stream;
  _stream << (p_align == tld::align::right ? std::right : std::left)
          << std::fixed << std::setfill(p_fill)
          << std::setw(tla::internal::max_str_length<uint8_t>())
          << static_cast<uint16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for int8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string format_number(int8_t p_num, char p_fill = '0',
                                 tld::align p_align = tld::align::right) {
  std::stringstream _stream;
  _stream << (p_align == tld::align::right ? std::right : std::left)
          << std::fixed << std::setfill(p_fill)
          << std::setw(tla::internal::max_str_length<uint8_t>())
          << static_cast<int16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
inline std::string format_number(t_num_type p_num, uint8_t p_size,
                                 char p_fill = '0',
                                 tld::align p_align = tld::align::right) {
  std::stringstream _stream;
  _stream << (p_align == tld::align::right ? std::right : std::left)
          << std::fixed << std::setfill(p_fill)
          << std::setw(p_size)
          //            << std::setprecision(tla::internal::max_str_length<long
          //            double>())
          << p_num;
  return _stream.str();
}

} // namespace tenacitas::lib::alg

namespace tla = tenacitas::lib::alg;

#endif
