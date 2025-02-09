/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRANSLATOR_TRANSLATOR_FROM_FILE_H
#define TNCT_TRANSLATOR_TRANSLATOR_FROM_FILE_H

#include <cstdint>
#include <fstream>
#include <regex>
#include <string>

#include "tnct/format/fmt.h"
#include "tnct/traits/logger.h"
#include "tnct/translator/dictionary.h"

namespace tnct::translator {

template <traits::logger t_logger, std::unsigned_integral t_index = uint32_t>
struct translator_from_file {
  using logger = t_logger;

  translator_from_file(logger &p_logger) : m_logger(p_logger) {}
  translator_from_file() = delete;
  translator_from_file(const translator_from_file &) = delete;
  translator_from_file(translator_from_file &&) = delete;
  ~translator_from_file() = default;

  translator_from_file &operator=(const translator_from_file &) = delete;
  translator_from_file &operator=(translator_from_file &&) = delete;

  uint32_t size() const { return m_dictionary.size(); }

  std::string get(t_index p_word_id) { return m_dictionary[p_word_id]; }

  bool load(const std::string &p_file_name) {
    const std::regex _regex{R"(^\s*(\d*)\s*\|(.*)$)"};

    decltype(m_dictionary) _dictionary;

    try {
      std::ifstream _file(p_file_name);
      if (!_file.good()) {

        m_logger.err(
            format::fmt("translation file '", p_file_name, "' not found"));

        return false;
      }

      std::string _line;

      std::getline(_file, _line);
      while (!_file.eof()) {
        if (_line.empty()) {
          continue;
        }

        std::smatch _matches;
        std::regex_search(_line, _matches, _regex);
        if (_matches.size() != 3) {
          continue;
        }
        _dictionary[std::stoul(_matches[1].str())] = _matches[2].str();
        std::getline(_file, _line);
      }
      m_dictionary = std::move(_dictionary);
      return true;

    } catch (std::exception &_ex) {

      m_logger.err(format::fmt("Loading translation file '", p_file_name,
                                "': '", _ex.what(), '\''));
    }
    return false;
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_from_file &p_translator) {
    for (const auto &p_value : p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  logger &m_logger;
  dictionary<t_index> m_dictionary;
};

} // namespace tnct::translator

#endif
