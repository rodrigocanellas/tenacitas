
#ifndef TNCT_INTERPRETER_BUS_STANDARD_SCANNERS_H
#define TNCT_INTERPRETER_BUS_STANDARD_SCANNERS_H

#include <cctype>
#include <optional>
#include <string>

namespace tnct::interpreter::bus {

/// Scans a word in the format:
///
/// word: letter |
///       letter word
///
/// letter: a | b ... | z | A | B ... | Z
class word_scanner {
public:
  std::pair<bool, std::string::const_iterator>
  operator()(std::string::const_iterator p_begin) const {
    std::string::const_iterator _ite{p_begin};

    if (!isalpha(*_ite)) {
      return {false, _ite};
    } else {
      while (true) {
        if (!isalpha(*_ite)) {
          break;
        }
        ++_ite;
      }
    }

    return {true, _ite};
  }
};

/// Scans a real number in the format:
///
/// real: digit-sequence  decimal-separator digit-sequence
///
/// digit-sequence: digit  |
///                 digit digit-sequence
///
/// digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
///
/// @note @p decimal-separator is defined in the constructor
class real_number_scanner {
public:
  real_number_scanner(char p_decimal_separator = '.');

  std::pair<bool, std::string::const_iterator>
  operator()(std::string::const_iterator p_begin) const {

    auto is_digit = [](std::string::const_iterator p_ite) -> bool {
      return (isdigit(*p_ite) != 0);
    };

    auto is_dec_sep = [&](std::string::const_iterator p_ite) -> bool {
      return (*p_ite == m_dec_sep);
    };

    bool _recognized{false};

    std::string::const_iterator _ite{p_begin};

    if (is_digit(_ite)) {
      ++_ite;
      goto digit_found_;
    }

    if (is_dec_sep(_ite)) {
      ++_ite;
      goto dec_sep_found_;
    }

    goto end_;

  digit_found_: {
    while (is_digit(_ite)) {
      ++_ite;
    }

    if (is_dec_sep(_ite)) {
      ++_ite;
      goto dec_sep_found_;
    }

    _recognized = true;
    goto end_;
  }

  dec_sep_found_: {
    if (!is_digit(_ite)) {
      goto end_;
    }

    while (is_digit(_ite)) {
      ++_ite;
    }

    _recognized = true;
    goto end_;
  }
  end_: { return {_recognized, _ite}; }
  }

private:
  char m_dec_sep;
};

/// Scans a decimal integer in the format:
///
/// decimal_integer: digit-sequence
///
/// digit-sequence: digit  |
///                 digit digit-sequence
///
/// digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
class decimal_integer_number_scanner {
public:
  std::optional<std::string::const_iterator>
  operator()(std::string::const_iterator p_begin) const {

    auto is_digit = [](std::string::const_iterator p_ite) -> bool {
      return (isdigit(*p_ite) != 0);
    };

    std::string::const_iterator _ite{p_begin};

    while (is_digit(_ite)) {
      ++_ite;
    }

    return {_ite};
  }
};

} // namespace tnct::interpreter::bus

#endif
