#ifndef TENACITAS_LOGGER_LEVEL_H
#define TENACITAS_LOGGER_LEVEL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

/// \brief The level enum defines the possible logger::log levels
enum class level : char {
  debug = 'D',
  info = 'I',
  warn = 'W',
  error = 'E',
  fatal = 'F',
  no_log = 'N'
};

inline std::ostream &operator<<(std::ostream &p_out, level p_level) {

  switch (p_level) {
  case level::debug:
    p_out << "DEB";
    break;
  case level::info:
    p_out << "INF";
    break;
  case level::warn:
    p_out << "WAR";
    break;
  case level::error:
    p_out << "ERR";
    break;
  case level::fatal:
    p_out << "FAT";
    break;
  default:
    p_out << "-N-";
  }
  return p_out;
}

level get_level();

void set_level(level p_level);

} // namespace logger
} // namespace tenacitas

#endif
