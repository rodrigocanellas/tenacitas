/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_DAT_LOG_LEVEL_H
#define TENACITAS_LIB_DAT_LOG_LEVEL_H

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::dat {

enum class log_level : int8_t {
  test = -2,
  no_log = -1,
  trace = 0,
  debug = 1,
  info = 2,
  warn = 3,
  error = 4,
  fatal = 5
};

/// \brief Output operator for \p level
///
/// \param p_out is the output stream
/// \param p_level is the level to be written to p_out
inline std::ostream &operator<<(std::ostream &p_out, log_level p_level) {
  switch (p_level) {
    case log_level::test:
    p_out << "TST";
    break;
    case log_level::trace:
    p_out << "TRA";
    break;
    case log_level::debug:
    p_out << "DEB";
    break;
    case log_level::info:
    p_out << "INF";
    break;
    case log_level::warn:
    p_out << "WAR";
    break;
    case log_level::error:
    p_out << "ERR";
    break;
    case log_level::fatal:
    p_out << "FAT";
    break;
  default:
    p_out << "***";
  }

  return p_out;
}

} // namespace tenacitas::lib::dat

namespace tld = tenacitas::lib::dat;

#endif
