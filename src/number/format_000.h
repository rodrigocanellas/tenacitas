#ifndef TENACITAS_NUMBER_FORMAT_NUMBER_H
#define TENACITAS_NUMBER_FORMAT_NUMBER_H

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <number/max_str_length.h>

namespace tenacitas {
namespace number {

enum class align : char { left = 'l', right = 'd' };

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
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
template <typename t_num_type>
std::string format_000(t_num_type p_num, char p_fill = '0',
                       align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<t_num_type>())
          << p_num;
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p tenacitas::number::format_000
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
std::string format_000(uint8_t p_num, char p_fill = '0',
                       align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
          << static_cast<uint16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for int8_t of \p tenacitas::number::format_000
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
std::string format_000(int8_t p_num, char p_fill = '0',
                       align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
          << static_cast<int16_t>(p_num);
  return _stream.str();
}

} // namespace number
} // namespace tenacitas

#endif