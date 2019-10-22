#ifndef TENACITAS_LOGGER_BUS_LEVEL_H
#define TENACITAS_LOGGER_BUS_LEVEL_H

#include <cstdint>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

///
/// \brief The level enum defines the possible log levels
///
enum class level : char
{
  debug = 'D',
  test = 'T',
  info = 'I',
  warn = 'W',
  error = 'E',
  fatal = 'F',
  no_log = 'N'
};

inline static const std::string & level2str(level p_level) {
  static const std::string _debug("DEB");
  static const std::string _test("TST");
  static const std::string _info("INF");
  static const std::string _warn("WAR");
  static const std::string _error("ERR");
  static const std::string _fatal("FAT");
  static const std::string _no_log("NO LOG");
  switch (p_level) {
    case level::debug:
      return _debug;
    case level::test:
      return _test;
    case level::info:
      return _info;
    case level::warn:
      return _warn;
    case level::error:
      return _error;
    case level::fatal:
      return _fatal;
    default:
      return _no_log;
  }
}

} // namespace business
} // namespace logger
} // namespace tenacitas

#endif
