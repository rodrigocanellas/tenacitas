/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_STRING_DAT_FIXED_SIZE_STRING_H
#define TNCT_STRING_DAT_FIXED_SIZE_STRING_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>

namespace tnct::string::dat {

template <std::size_t t_size> struct fixed_size_string {
  // static constexpr decltype(t_size) size = t_size;

private:
  using array = std::array<char, t_size + 1>;
  array m_value{'\0'};

public:
  using const_iterator = array::const_iterator;

  fixed_size_string() = default;

  // with help from ChatGPT
  template <std::uint16_t t_str_size>
  constexpr fixed_size_string(const char (&p_value)[t_str_size]) {

    static_assert(t_str_size <= t_size + 1,
                  "string literal is too large for fixed_size_string");

    std::copy_n(p_value, std::strlen(p_value), m_value.begin());
  }

  constexpr fixed_size_string(const_iterator p_begin, const_iterator p_end) {
    if (static_cast<decltype(t_size)>(std::distance(p_begin, p_end)) >
        t_size + 1) {
      throw std::runtime_error(
          "string too large when creating a fixed_size_string!");
    }

    std::copy_n(p_begin, std::distance(p_begin, p_end), m_value.begin());
  }

  // with help from ChatGPT
  fixed_size_string(std::string_view p_value) {
    // reset();
    if (p_value.size() > t_size) {
      // Triggers a compile-time error
      throw std::runtime_error(
          "string too large when creating a fixed_size_string!");
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
    const auto _size{size()};
    if (p_fixed_size_string.size() != _size) {
      return false;
    }
    for (decltype(t_size) _i = 0; _i < _size; ++_i) {
      if ((p_fixed_size_string.m_value[_i] != m_value[_i])) {
        return false;
      }
    }
    return true;
  }

  constexpr bool
  operator!=(const fixed_size_string &p_fixed_size_string) const {
    return !(*this == p_fixed_size_string);
  }

  constexpr bool operator<(const fixed_size_string &p_fixed_size_string) const {

    static int _k{0};

    std::cout << __FILE__ << ':' << __LINE__ << " _k = " << _k++ << std::endl;

    decltype(t_size) _size{size()};
    if (_size > p_fixed_size_string.size()) {
      _size = p_fixed_size_string.size();
    }

    std::cout << __FILE__ << ':' << __LINE__ << " *this " << *this
              << ", p_fixed_size_string " << p_fixed_size_string
              << ", size() = " << size()
              << ", p_fixed_size_string.size() = " << p_fixed_size_string.size()
              << ", _size = " << _size << std::endl;

    for (decltype(t_size) _i = 0; _i < _size; ++_i) {
      std::cout << __FILE__ << ':' << __LINE__
                << " 'p_fixed_size_string.m_value[_i]' "
                << p_fixed_size_string.m_value[_i] << ", 'm_value[_i]' "
                << m_value[_i] << std::endl;
      if ((static_cast<std::size_t>(m_value[_i]) >
           static_cast<std::size_t>(p_fixed_size_string.m_value[_i]))) {
        std::cout << __FILE__ << ':' << __LINE__ << " returning false"
                  << std::endl;
        return false;
      }
    }

    if (size() > p_fixed_size_string.size()) {
      std::cout << __FILE__ << ':' << __LINE__ << " returning false\n";
      return false;
    }
    std::cout << __FILE__ << ':' << __LINE__ << " returning true\n";
    return true;
  }

  constexpr bool empty() const { return m_value[0] == '\0'; }

  constexpr decltype(t_size) size() const {
    return std::distance(this->begin(), this->end());
  }

  constexpr operator std::string() const { return std::string{m_value.data()}; }

  constexpr const_iterator begin() const { return m_value.begin(); }

  constexpr const_iterator end() const {
    const_iterator _ite{m_value.begin()};
    while (*_ite != '\0') {
      ++_ite;
    }
    //    return std::find(m_value.begin(), m_value.end(), '\0');
    return _ite;
  }

  constexpr bool equals(const_iterator p_begin, const_iterator p_end) const {
    const auto _distance{
        static_cast<decltype(t_size)>(std::distance(p_begin, p_end))};
    if (_distance >= (t_size + 1)) {
      return false;
    }

    if (empty() || (p_begin == p_end)) {
      return false;
    }

    const auto _size{size()};

    if (_distance != _size) {
      return false;
    }

    const_iterator _ite{p_begin};

    for (decltype(t_size) _i = 0; _i < _size; ++_i) {
      if (*_ite != m_value[_i]) {
        return false;
      }
      ++_ite;
    }

    return true;
  }

  friend std::ostream &
  operator<<(std::ostream &p_out,
             const fixed_size_string &p_fixed_size_string) {
    p_out << p_fixed_size_string.m_value.data();
    return p_out;
  };
};

} // namespace tnct::string::dat

#endif
