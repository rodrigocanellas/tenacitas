#ifndef TNCT_CROSSWORDS_DAT_ERRORS_H
#define TNCT_CROSSWORDS_DAT_ERRORS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string>

namespace tnct::crosswords::dat
{

enum class error_code : uint16_t
{
  NO_ERROR      = 0,
  WORD_TOO_LONG = 1
};

struct error
{
  error()              = default;
  error(const error &) = default;
  error(error &&)      = default;
  error(error_code p_code, std::string p_description)
      : code(p_code), description(p_description)
  {
  }
  ~error() = default;

  error &operator=(const error &) = default;
  error &operator=(error &&)      = default;

  friend std::ostream &operator<<(std::ostream &p_out, const error &p_error)
  {
    p_out << "{";
    switch (p_error.code)
    {
    case error_code::NO_ERROR:
      p_out << "none";
      break;
    case error_code::WORD_TOO_LONG:
      p_out << "word too long";
      break;
    }
    p_out << ", " << p_error.description << '}';
    return p_out;
  }

  error_code  code{error_code::NO_ERROR};
  std::string description;
};
} // namespace tnct::crosswords::dat

#endif
