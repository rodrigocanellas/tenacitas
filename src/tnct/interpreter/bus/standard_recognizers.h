
#ifndef TNCT_INTERPRETER_BUS_STANDARD_recognizerS_H
#define TNCT_INTERPRETER_BUS_STANDARD_recognizerS_H

#include <cctype>
#include <optional>
#include <string>

#include "tnct/interpreter/dat/non_terminal_recognition.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

inline bool is_alpha(char p_char) {
  return std::isalpha(static_cast<unsigned char>(p_char)) != 0;
}

inline bool is_digit(char p_char) {
  return std::isdigit(static_cast<unsigned char>(p_char)) != 0;
}

/// Recognizes a word in the format:
///
/// word: letter |
///       letter word
///
/// letter: a | b ... | z | A | B ... | Z
class word_recognizer final {
public:
  explicit word_recognizer(dat::type p_type) : m_type(p_type) {}

  word_recognizer() = default;
  word_recognizer(const word_recognizer &) = default;
  word_recognizer(word_recognizer &&) = default;

  ~word_recognizer() = default;

  void *operator new(std::size_t) = delete;
  void *operator new[](std::size_t) = delete;

  word_recognizer &operator=(const word_recognizer &) = default;

  word_recognizer &operator=(word_recognizer &&) = default;

  std::optional<dat::non_terminal_recognition>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) {
    std::string::const_iterator _ite{p_begin};

    auto stop = [&]() { return (_ite == p_end) || !is_alpha(*_ite); };

    if (stop()) {
      return std::nullopt;
    }

    while (!stop()) {
      ++_ite;
    }

    dat::non_terminal _nt{{p_begin, _ite}, m_type};

    return {dat::non_terminal_recognition{.end{_ite},
                                          .non_terminal{std::move(_nt)}}};
  }

private:
  dat::type m_type{dat::unknow_type};
};

/// Recognizes a real number in the format:
///
/// real: digit-sequence decimal-separator digit-sequence
///
/// digit-sequence: digit |
///                 digit digit-sequence
///
/// digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
///
/// @note @p decimal-separator is defined in the constructor.
class real_number_recognizer final {
public:
  explicit real_number_recognizer(dat::type p_type,
                                  char p_decimal_separator = '.')
      : m_type(p_type), m_dec_sep(p_decimal_separator) {}

  real_number_recognizer() = default;
  real_number_recognizer(const real_number_recognizer &) = default;
  real_number_recognizer(real_number_recognizer &&) = default;
  ~real_number_recognizer() = default;

  void *operator new(std::size_t) = delete;
  void *operator new[](std::size_t) = delete;

  real_number_recognizer &operator=(const real_number_recognizer &) = default;
  real_number_recognizer &operator=(real_number_recognizer &&) = default;

  std::optional<dat::non_terminal_recognition>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) const {
    std::string::const_iterator _ite{p_begin};

    auto consume_digits = [&]() {
      bool _found{false};
      while ((_ite != p_end) && is_digit(*_ite)) {
        _found = true;
        ++_ite;
      }
      return _found;
    };

    const bool _has_integer_part{consume_digits()};

    if (!_has_integer_part || (_ite == p_end) || (*_ite != m_dec_sep)) {
      return std::nullopt;
    }

    ++_ite;

    const bool _has_fractional_part{consume_digits()};

    if (!_has_fractional_part) {
      return std::nullopt;
    }

    return dat::non_terminal_recognition{
        .end = _ite,
        .non_terminal = {.value = {p_begin, _ite}, .type = m_type}};
  }

private:
  dat::type m_type{dat::unknow_type};
  char m_dec_sep{'.'};
};

/// Recognizes a decimal integer in the format:
///
/// decimal_integer: digit-sequence
///
/// digit-sequence: digit  |
///                 digit digit-sequence
///
/// digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
class decimal_integer_number_recognizer {
public:
  explicit decimal_integer_number_recognizer(dat::type p_type)
      : m_type(p_type) {}

  decimal_integer_number_recognizer() = default;
  decimal_integer_number_recognizer(const decimal_integer_number_recognizer &) =
      default;
  decimal_integer_number_recognizer(decimal_integer_number_recognizer &&) =
      default;

  ~decimal_integer_number_recognizer() = default;

  void *operator new(std::size_t) = delete;
  void *operator new[](std::size_t) = delete;

  decimal_integer_number_recognizer &
  operator=(const decimal_integer_number_recognizer &) = default;

  decimal_integer_number_recognizer &
  operator=(decimal_integer_number_recognizer &&) = default;

  std::optional<dat::non_terminal_recognition>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) {

    std::string::const_iterator _ite{p_begin};

    auto end = [&]() -> bool { return (_ite == p_end); };

    if (end() || !is_digit(*_ite)) {
      return std::nullopt;
    }

    while (!end() && is_digit(*_ite)) {
      ++_ite;
    }

    return {dat::non_terminal_recognition{
        .end = _ite,
        .non_terminal = {.value = {p_begin, _ite}, .type = m_type}}};
  }

private:
  dat::type m_type{dat::unknow_type};
};

} // namespace tnct::interpreter::bus

#endif
