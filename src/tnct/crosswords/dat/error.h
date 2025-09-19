#ifndef TNCT_CROSSWORDS_DAT_ERRORS_H
#define TNCT_CROSSWORDS_DAT_ERRORS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

namespace tnct::crosswords::dat
{

enum class error : uint16_t
{
  NO_ERROR                   = 0,
  WORD_TOO_LONG              = 1,
  INVALID_INTERVAL           = 2,
  MAX_ROWS_SMALLER_THAN_ROWS = 3
};

inline std::ostream &operator<<(std::ostream &p_out, error p_error)
{
  switch (p_error)
  {
  case error::NO_ERROR:
    p_out << "none";
    break;

  case error::WORD_TOO_LONG:
    p_out << "word too long";
    break;

  case error::INVALID_INTERVAL:
    p_out << "invalid interval";
    break;

  case error::MAX_ROWS_SMALLER_THAN_ROWS:
    p_out << "max rows is smaller that number of rows";
    break;
  };
  return p_out;
}

} // namespace tnct::crosswords::dat

#endif
