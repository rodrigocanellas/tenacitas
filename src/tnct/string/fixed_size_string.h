/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_STRING_FIXED_SIZE_STRING_H
#define TNCT_STRING_FIXED_SIZE_STRING_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>

namespace tnct::string {

template <std::uint16_t t_size> struct fixed_size_string {
  static constexpr std::uint16_t size = t_size;

  fixed_size_string() = default;

  // with help from ChatGPT
  template <std::uint16_t t_str_size>
  constexpr fixed_size_string(const char (&p_value)[t_str_size]) {

    static_assert(t_str_size <= t_size + 1,
                  "string literal is too large for fixed_size_string");

    std::copy_n(p_value, std::strlen(p_value), m_value.begin());
  }

  // with help from ChatGPT
  fixed_size_string(std::string_view p_value) {
    // reset();
    if (p_value.size() > t_size) {
      throw std::runtime_error(
          "string too large!"); // Triggers a compile-time error
    }
    std::copy_n(p_value.begin(), p_value.size(), m_value.begin());
  }

  fixed_size_string(const fixed_size_string &p_fixed_size_string)
      : m_value(p_fixed_size_string.m_value) {}

  fixed_size_string(fixed_size_string &&p_fixed_size_string)
      : m_value(std::move(p_fixed_size_string.m_value)) {}

  fixed_size_string &operator=(fixed_size_string &&p_fixed_size_string) {
    if (this != &p_fixed_size_string) {
      m_value = std::move(p_fixed_size_string.m_value);
    }
    return *this;
  }

  fixed_size_string &operator=(const fixed_size_string &p_fixed_size_string) {
    if (this != &p_fixed_size_string) {
      m_value = p_fixed_size_string.m_value;
    }
    return *this;
  }

  constexpr bool
  operator==(const fixed_size_string &p_fixed_size_string) const {
    for (decltype(t_size) _i = 0; _i < t_size; ++_i) {

      if ((m_value[_i] != '\0') &&
          (p_fixed_size_string.m_value[_i] != m_value[_i])) {
        return false;
      }
    }
    return true;
  }

  constexpr bool operator<(const fixed_size_string &p_fixed_size_string) const {
    for (decltype(t_size) _i = 0; _i < t_size; ++_i) {

      if ((m_value[_i] != '\0') &&
          (p_fixed_size_string.m_value[_i] >= m_value[_i])) {
        return false;
      }
    }
    return true;
  }

  constexpr bool empty() const { return m_value[0] == '\0'; }

  constexpr bool
  operator!=(const fixed_size_string &p_fixed_size_string) const {
    return !(m_value == p_fixed_size_string.m_value);
  }

  friend std::ostream &
  operator<<(std::ostream &p_out,
             const fixed_size_string &p_fixed_size_string) {
    p_out << p_fixed_size_string.m_value.data();
    return p_out;
  };

private:
  std::array<char, t_size + 1> m_value{'\0'};
};

} // namespace tnct::string

#endif
