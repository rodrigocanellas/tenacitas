/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_ALG_TRANSLATOR_IN_MEMORY_H
#define TNCT_LIB_ALG_TRANSLATOR_IN_MEMORY_H

#include <cstdint>
#include <string>

#include <tnct/lib/dat/dictionary.h>
#ifdef TENACITAS_LOG
#include <tnct/lib/alg/log.h>
#endif

namespace tnct::lib::alg {

template <std::unsigned_integral t_index = uint32_t>
struct translator_in_memory_a {
  translator_in_memory_a() = default;
  translator_in_memory_a(tld::dictionary<t_index> &&p_dictionary)
      : m_dictionary(std::move(p_dictionary)) {}
  translator_in_memory_a(const translator_in_memory_a &) = delete;
  translator_in_memory_a(translator_in_memory_a &&) = delete;
  ~translator_in_memory_a() = default;

  translator_in_memory_a &operator=(const translator_in_memory_a &) = delete;
  translator_in_memory_a &operator=(translator_in_memory_a &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  inline std::string get(t_index p_word_id) { return m_dictionary[p_word_id]; }

  void inline add(t_index p_entry, std::string_view p_str) {
    m_dictionary[p_entry] = p_str;
  }

  void inline add(tld::dictionary<t_index> &&p_dictionary) {
    for (const auto &_value : p_dictionary) {
      m_dictionary[_value.first] = _value.second;
    }
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_in_memory_a &p_translator) {
    for (const auto &p_value : p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  tld::dictionary<t_index> m_dictionary;
};

} // namespace tnct::lib::alg

#endif
