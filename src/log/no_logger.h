/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_NO_LOGGER_H
#define TNCT_LOG_NO_LOGGER_H

#include <source_location>
#include <string_view>

namespace tenacitas::src::log {

struct no_logger {
  no_logger() = default;
  no_logger(const no_logger &) = delete;

  no_logger(no_logger &&) = delete;

  void *operator new(size_t) = delete;

  void tra(std::string_view,
           std::source_location = std::source_location::current()) {}

  void deb(std::string_view,
           std::source_location = std::source_location::current()) {}

  void inf(std::string_view,
           std::source_location = std::source_location::current()) {}

  void war(std::string_view,
           std::source_location = std::source_location::current()) {}

  void err(std::string_view,
           std::source_location = std::source_location::current()) {}

  void fat(std::string_view,
           std::source_location = std::source_location::current()) {}

  void set_tra() {}

  void set_deb() {}

  void set_inf() {}

  void set_war() {}

  void save_level() {}

  void restore_level() {}

  bool can_tra() { return false; }

  bool can_deb() { return false; }

  bool can_inf() { return false; }

  bool can_war() { return false; }
};

} // namespace tenacitas::src::log

#endif
