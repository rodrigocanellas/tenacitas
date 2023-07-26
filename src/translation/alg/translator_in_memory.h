#ifndef TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_IN_MEMORY_H
#define TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_IN_MEMORY_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

#include <tenacitas.lib/src/translation/alg/translator.h>
#include <tenacitas.lib/src/translation/typ/types.h>

namespace tenacitas::lib::translation::alg {

struct translator_in_memory {
  translator_in_memory() = default;
  translator_in_memory(typ::dictionary &&p_dictionary)
      : m_dictionary(std::move(p_dictionary)) {}
  translator_in_memory(const translator_in_memory &) = delete;
  translator_in_memory(translator_in_memory &&) = delete;
  ~translator_in_memory() = default;

  translator_in_memory &operator=(const translator_in_memory &) = delete;
  translator_in_memory &operator=(translator_in_memory &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  std::string inline get(typ::entry p_word_id) {
    return m_dictionary[p_word_id];
  }

  void inline add(typ::entry p_entry, std::string_view p_str) {
    m_dictionary[p_entry] = p_str;
  }

  void inline add(typ::dictionary &&p_dictionary) {
    for (const typ::dictionary::value_type &_value : p_dictionary) {
      m_dictionary[_value.first] = _value.second;
    }
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_in_memory &p_translator) {
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
