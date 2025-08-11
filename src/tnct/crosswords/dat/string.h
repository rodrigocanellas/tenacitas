#ifndef TENACITAS_SRC_CROSSWORDS_DAT_STRING_H
#define TENACITAS_SRC_CROSSWORDS_DAT_STRING_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <limits>
#include <string>

#include "tenacitas/src/crosswords/dat/index.h"

#include <QString>

namespace tenacitas::src::crosswords::dat {

struct string {
  using value_type = QChar;

  string() = default;
  string(const value_type *p_value) : m_value(p_value) {}
  string(const char *p_value) : m_value(p_value) {}
  string(const string &p_word) : m_value(p_word.m_value) {}
  string(const std::string &p_word) : m_value(p_word.c_str()) {}
  string(string &&p_word) : m_value(std::move(p_word.m_value)) {}
  string(std::string &&p_word) : m_value(p_word.c_str()) {}

  string &operator=(const string &p_word) {
    if (this != &p_word) {
      m_value = p_word.m_value;
    }
    return *this;
  }

  string &operator=(string &&p_word) {
    if (this != &p_word) {
      m_value = std::move(p_word.m_value);
    }
    return *this;
  }

  string &operator=(const std::string &p_word) {
    m_value = QString{p_word.c_str()};

    return *this;
  }

  string &operator=(std::string &&p_word) {
    m_value = QString{p_word.c_str()};
    return *this;
  }

  ~string() = default;

  friend std::ostream &operator<<(std::ostream &p_out, const string &p_word) {
    p_out << p_word.m_value.toStdString();
    return p_out;
  }

  index size() const { return static_cast<index>(m_value.size()); }

  value_type operator[](size_t idx) const {
    return m_value[static_cast<qsizetype>(idx)];
  }

  bool operator<(const string &p_word) const {
    if (size() < p_word.size()) {
      return true;
    }

    if (size() > p_word.size()) {
      return false;
    }

    for (index _i = 0; _i < size(); ++_i) {
      if (m_value[_i] < p_word[_i]) {
        return true;
      }
    }
    return false;
  }

  std::string to_string() const { return m_value.toStdString(); }

  static constexpr string::value_type max_char{
      std::numeric_limits<unsigned short>::max()};

private:
  QString m_value;
};

// using word1 = QString;

inline std::ostream &operator<<(std::ostream &p_out,
                                string::value_type p_char) {
  p_out << QString(p_char).toStdString();
  return p_out;
}

/// \brief Maximum length of a \p string

} // namespace tenacitas::src::crosswords::dat

#endif
