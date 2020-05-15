#ifndef TENACITAS_LOGGER_BUS_LEVEL_H
#define TENACITAS_LOGGER_BUS_LEVEL_H

#include <cstdint>
#include <string>

//#define RESET std::string("\033[0m")
//#define BLACK std::string("\033[30m")   /* Black */
//#define RED std::string("\033[31m")     /* Red */
//#define GREEN std::string("\033[32m")   /* Green */
//#define YELLOW std::string("\033[33m")  /* Yellow */
//#define BLUE std::string("\033[34m")    /* Blue */
//#define MAGENTA std::string("\033[35m") /* Magenta */
//#define CYAN std::string("\033[36m")    /* Cyan */
//#define WHITE std::string("\033[37m")   /* White */

//#define BKG_WHITE std::string("\033[47m") /* White */

//#define BOLDBLACK std::string("\033[1m\033[30m")   /* Bold Black */
//#define BOLDRED std::string("\033[1m\033[31m")     /* Bold Red */
//#define BOLDGREEN std::string("\033[1m\033[32m")   /* Bold Green */
//#define BOLDYELLOW std::string("\033[1m\033[33m")  /* Bold Yellow */
//#define BOLDBLUE std::string("\033[1m\033[34m")    /* Bold Blue */
//#define BOLDMAGENTA std::string("\033[1m\033[35m") /* Bold Magenta */
//#define BOLDCYAN std::string("\033[1m\033[36m")    /* Bold Cyan */
//#define BOLDWHITE std::string("\033[1m\033[37m")   /* Bold White */

//   For a colored background, reset = 0, black = 40, red = 41, green = 42,
//   yellow = 43, blue = 44, magenta = 45, cyan = 46, and white=47, are the
//   commonly used color codes.

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

//
/// \brief The level enum defines the possible logger::log levels
//
enum class level : char {
  debug = 'D',
  info = 'I',
  warn = 'W',
  error = 'E',
  fatal = 'F',
  no_log = 'N'
};

static const std::string &level2str(level p_level) {
  // static const std::string _debug(BOLDBLACK + "DEB" + RESET);
  // static const std::string _test(BOLDGREEN + "TST" + RESET);
  // static const std::string _info(BOLDCYAN + "INF" + RESET);
  // static const std::string _warn(BOLDBLUE + "WAR" + RESET);
  // static const std::string _error(BOLDMAGENTA + "ERR" + RESET);
  // static const std::string _fatal(BOLDRED + "FAT" + RESET);

  static const std::string _debug("DEB");
  static const std::string _info("INF");
  static const std::string _warn("WAR");
  static const std::string _error("ERR");
  static const std::string _fatal("FAT");
  static const std::string _no_log("NO LOG");

  switch (p_level) {
  case level::debug:
    return _debug;
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

} // namespace logger
} // namespace tenacitas

#endif
