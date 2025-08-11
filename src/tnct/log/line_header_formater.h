/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_LINE_HEADER_FORMATER_H
#define TNCT_LOG_LINE_HEADER_FORMATER_H

#include <chrono>
#include <cstring>
#include <filesystem>
#include <functional>
#include <source_location>
#include <sstream>
#include <string>
#include <thread>

#include "tnct/format/format_number.h"
#include "tnct/log/level.h"

namespace tnct::log {

using line_header_formater = std::function<std::stringstream &(
    std::stringstream &, level, const std::source_location &)>;

static inline std::stringstream &
default_line_header_formater(std::stringstream &p_stream, level p_level,
                             const std::source_location &p_source_location) {
  static constexpr char _separator = '|';
  static constexpr uint8_t _max_file_name_size = 35;

  const double _now_microsecs =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
  const time_t _now_seconds{static_cast<time_t>(_now_microsecs / 1000000)};

  // _time_str must have space enough for YYYY-MM-DD HH:MM:SS
  char _time_str[4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 1];
  strftime(_time_str, sizeof _time_str, "%Y-%m-%d %H:%M:%S",
           localtime(&_now_seconds));

  auto _aux(
      std::filesystem::path(p_source_location.file_name()).filename().string());

  decltype(_aux) _file_name;

  if (_aux.size() > _max_file_name_size) {
    _file_name.append(&_aux[0], &_aux[_max_file_name_size - 3]);
    _file_name.append("...");
  } else {
    _file_name = _aux;
  }

  p_stream << p_level << _separator << _time_str << ','
           << format::format_fix_number(
                  static_cast<uint32_t>(_now_microsecs -
                                        (_now_seconds * 1000000)),
                  6, '0', tnct::format::align::right)
           << _separator << std::this_thread::get_id() << _separator
           << std::setfill(' ') << std::left << std::setw(_max_file_name_size)
           << _file_name << _separator
           << format::format_fix_number(p_source_location.line(), uint8_t{5})
           // << _separator << p_source_location.function_name()

           << _separator;
  return p_stream;
}

} // namespace tnct::log

#endif
