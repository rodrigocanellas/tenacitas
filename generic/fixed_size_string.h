/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_FIXED_SIZE_STRING_H
#define TENACITAS_LIB_FIXED_SIZE_STRING_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string_view>

namespace tenacitas::lib::generic {

template <size_t t_size = 10> struct fixed_size_string {
  using const_iterator = const char *;

  fixed_size_string() = delete;

  fixed_size_string(std::string_view p_value) {
    memset(m_value, '\0', t_size + 1);
    if (p_value.size() <= t_size) {
      std::copy(p_value.begin(), p_value.end(), &m_value[0]);
    } else {
      std::copy_n(p_value.begin(), t_size, &m_value[0]);
    }
  }

  fixed_size_string(const char *p_value) {
    memset(m_value, '\0', t_size + 1);
    auto _size(strlen(p_value));
    if (_size <= t_size) {
      std::copy(&p_value[0], &p_value[_size], &m_value[0]);
    } else {
      std::copy_n(&p_value[0], t_size, &m_value[0]);
    }
  }

  fixed_size_string(const fixed_size_string &p_fixed_string) {
    memset(m_value, '\0', t_size + 1);
    strcpy(&m_value[0], &p_fixed_string.m_value[0]);
  }

  fixed_size_string(fixed_size_string &&p_fixed_string) {
    memset(m_value, '\0', t_size + 1);
    strcpy(&m_value[0], &p_fixed_string.m_value[0]);
    memset(p_fixed_string.m_value, '\0', t_size + 1);
  }

  ~fixed_size_string() = default;

  fixed_size_string &operator=(const fixed_size_string &p_fixed_string) {
    if (this != &p_fixed_string) {
      memset(m_value, '\0', t_size + 1);
      strcpy(&m_value[0], &p_fixed_string.m_value[0]);
    }
    return *this;
  }

  fixed_size_string &operator=(fixed_size_string &&p_fixed_string) {
    if (this != &p_fixed_string) {
      memset(m_value, '\0', t_size + 1);
      strcpy(&m_value[0], &p_fixed_string.m_value[0]);
      memset(p_fixed_string.m_value, '\0', t_size + 1);
    }
    return *this;
  }

  bool operator==(const fixed_size_string &p_fixed_string) const {
    auto _size(strlen(m_value));
    if (_size != strlen(p_fixed_string.m_value)) {
      return false;
    }

    for (decltype(_size) _i = 0; _i < strlen(m_value); ++_i) {
      if (m_value[_i] != p_fixed_string.m_value[_i]) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const fixed_size_string &p_fixed_string) const {
    return !(*this == p_fixed_string);
  }

  bool operator<(const fixed_size_string &p_fixed_string) const {
    auto _size_1(strlen(m_value));
    auto _size_2(strlen(p_fixed_string.m_value));

    decltype(_size_1) _i(0);

    bool _less(true);
    while (true) {
      if (_i == _size_1) {
        break;
      }
      if (_i == _size_2) {
        break;
      }

      if (m_value[_i] > p_fixed_string.m_value[_i]) {
      }

      ++_i;
    }
    return _less;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const fixed_size_string &p_fixed_string) {
    p_out << p_fixed_string.m_value;
    return p_out;
  }

  const_iterator begin() const { return &m_value[0]; }
  const_iterator end() const { return &m_value[strlen(m_value)]; }

  size_t size() const { return strlen(m_value); }

  operator std::string() { return std::string(m_value); }

private:
  char m_value[t_size + 1];
};

} // namespace tenacitas::lib::generic

#endif
