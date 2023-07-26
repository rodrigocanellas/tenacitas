#ifndef TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_FROM_FILE_H
#define TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_FROM_FILE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>

#ifdef TENACITAS_LOG
#include <tenacitas.lib/src/log/alg/logger.h>
#endif

#include <tenacitas.lib/src/translation/alg/translator.h>
#include <tenacitas.lib/src/translation/typ/types.h>

namespace tenacitas::lib::translation::alg {

struct translator_from_file {
  translator_from_file() = default;
  translator_from_file(const translator_from_file &) = delete;
  translator_from_file(translator_from_file &&) = delete;
  ~translator_from_file() = default;

  translator_from_file &operator=(const translator_from_file &) = delete;
  translator_from_file &operator=(translator_from_file &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  std::string inline get(typ::entry p_word_id) {
    return m_dictionary[p_word_id];
  }

  bool load(const std::string &p_file_name) {
    const std::regex _regex{R"(^\s*(\d*)\s*\|(.*)$)"};

    typ::dictionary _dictionary;

    try {
      std::ifstream _file(p_file_name);
      if (!_file.good()) {
#ifdef TENACITAS_LOG
        TNCT_LOG_ERR("translation file '", p_file_name, "' not found");
#endif
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
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("Loading translation file '", p_file_name, "': '",
                   _ex.what(), '\'');
#endif
    }
    return false;
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_from_file &p_translator) {
    for (const typ::dictionary::value_type &p_value :
         p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  typ::dictionary m_dictionary;
};

} // namespace tenacitas::lib::translation::alg

#endif
