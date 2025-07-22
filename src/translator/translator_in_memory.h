/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRANSLATOR_TRANSLATOR_IN_MEMORY_H
#define TENACITAS_SRC_TRANSLATOR_TRANSLATOR_IN_MEMORY_H

#include <cstdint>
#include <string>

#include "tenacitas/src/translator/dictionary.h"

namespace tenacitas::src::translator {

template <std::unsigned_integral t_index = uint32_t>
struct translator_in_memory {
  translator_in_memory() = default;
  translator_in_memory(dictionary<t_index> &&p_dictionary)
      : m_dictionary(std::move(p_dictionary)) {}
  translator_in_memory(const translator_in_memory &) = delete;
  translator_in_memory(translator_in_memory &&) = delete;
  ~translator_in_memory() = default;

  translator_in_memory &operator=(const translator_in_memory &) = delete;
  translator_in_memory &operator=(translator_in_memory &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  inline std::string get(t_index p_word_id) { return m_dictionary[p_word_id]; }

  void inline add(t_index p_entry, std::string_view p_str) {
    m_dictionary[p_entry] = p_str;
  }

  void inline add(dictionary<t_index> &&p_dictionary) {
    for (const auto &_value : p_dictionary) {
      m_dictionary[_value.first] = _value.second;
    }
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_in_memory &p_translator) {
    for (const auto &p_value : p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  dictionary<t_index> m_dictionary;
};

} // namespace tenacitas::src::translator

#endif
