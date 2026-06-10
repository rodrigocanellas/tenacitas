
#ifndef TNCT_INTERPRETER_BUS_STANDARD_recognizerS_H
#define TNCT_INTERPRETER_BUS_STANDARD_recognizerS_H

#include <cctype>
#include <optional>
#include <string>

#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

/// Recognizes a word in the format:
///
/// word: letter |
///       letter word
///
/// letter: a | b ... | z | A | B ... | Z
class word_recognizer final {
public:
  explicit word_recognizer(dat::type p_type) : m_type(p_type) {}

  word_recognizer() = delete;
  word_recognizer(const word_recognizer &) = delete;
  word_recognizer(word_recognizer &&) = default;

  ~word_recognizer() = default;

  void *operator new(std::size_t) = delete;

  word_recognizer &operator=(const word_recognizer &) = delete;

  word_recognizer &operator=(word_recognizer &&) = delete;

  std::optional<std::pair<dat::type, std::string::const_iterator>>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) const {
    std::string::const_iterator _ite{p_begin};

    auto stop = [&]() { return (_ite == p_end) || !isalpha(*_ite); };

    if (stop()) {
      return std::nullopt;
    }

    while (!stop()) {
      ++_ite;
    }

    return {{m_type, _ite}};
  }

private:
  dat::type m_type{dat::unknow_type};
};

/// Recognizes a real number in the format:
///
/// real: digit-sequence  decimal-separator digit-sequence
///
/// digit-sequence: digit  |
///                 digit digit-sequence
///
/// digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
///
/// @note @p decimal-separator is defined in the constructor
class real_number_recognizer {
public:
  explicit real_number_recognizer(dat::type p_type,
                                  char p_decimal_separator = '.')
      : m_type(p_type), m_dec_sep(p_decimal_separator) {}

  real_number_recognizer() = delete;
  real_number_recognizer(const real_number_recognizer &) = delete;
  real_number_recognizer(real_number_recognizer &&) = default;

  ~real_number_recognizer() = default;

  void *operator new(std::size_t) = delete;

  real_number_recognizer &operator=(const real_number_recognizer &) = delete;

  real_number_recognizer &operator=(real_number_recognizer &&) = delete;

  std::optional<std::pair<dat::type, std::string::const_iterator>>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) const {

    std::string::const_iterator _ite{p_begin};

    auto is_digit = [&]() -> bool { return (isdigit(*_ite) != 0); };

    auto is_dec_sep = [&]() -> bool { return (*_ite == m_dec_sep); };

    auto end = [&]() { return _ite == p_end; };

    bool _recognized{false};

    if (_ite == p_end) {
      goto end_;
    }

    if (is_digit()) {
      ++_ite;
      goto digit_found_;
    }

    if (is_dec_sep()) {
      ++_ite;
      goto dec_sep_found_;
    }

    goto end_;

  digit_found_: {
    while (!end() && is_digit()) {
      ++_ite;
    }

    if (end()) {
      goto end_;
    }

    if (is_dec_sep()) {
      ++_ite;
      goto dec_sep_found_;
    }

    _recognized = true;
    goto end_;
  }

  dec_sep_found_: {
    if (end() || !is_digit()) {
      goto end_;
    }

    while (!end() && is_digit()) {
      ++_ite;
    }

    _recognized = true;
    goto end_;
  }
  end_: {
    if (_recognized) {
      return {{m_type, _ite}};
    }
    return std::nullopt;
  }
  }

private:
  dat::type m_type{dat::unknow_type};
  char m_dec_sep;
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

  decimal_integer_number_recognizer() = delete;
  decimal_integer_number_recognizer(const decimal_integer_number_recognizer &) =
      delete;
  decimal_integer_number_recognizer(decimal_integer_number_recognizer &&) =
      default;

  ~decimal_integer_number_recognizer() = default;

  void *operator new(std::size_t) = delete;

  decimal_integer_number_recognizer &
  operator=(const decimal_integer_number_recognizer &) = delete;

  decimal_integer_number_recognizer &
  operator=(decimal_integer_number_recognizer &&) = delete;

  std::optional<std::pair<dat::type, std::string::const_iterator>>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end) const {

    std::string::const_iterator _ite{p_begin};

    auto is_digit = [&]() -> bool { return (isdigit(*_ite) != 0); };
    auto end = [&]() -> bool { return (_ite == p_end); };

    if (end() || !is_digit()) {
      return std::nullopt;
    }

    while (!end() && is_digit()) {
      ++_ite;
    }

    return {{m_type, _ite}};
  }

private:
  dat::type m_type{dat::unknow_type};
};

} // namespace tnct::interpreter::bus

#endif
