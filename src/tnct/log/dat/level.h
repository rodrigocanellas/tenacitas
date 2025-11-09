/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_LEVEL_H
#define TNCT_LOG_LEVEL_H

#include <cstdint>
#include <iostream>

namespace tnct::log::dat
{

enum class level : int8_t
{
  tst    = -2,
  no_log = -1,
  tra    = 0,
  deb    = 1,
  inf    = 2,
  war    = 3,
  err    = 4,
  fat    = 5
};

/// \brief Output operator for \p level
///
/// \param p_out is the output stream
/// \param p_level is the level to be written to p_out
inline std::ostream &operator<<(std::ostream &p_out, level p_level)
{
  switch (p_level)
  {
  case level::tst:
    p_out << "TST";
    break;
  case level::tra:
    p_out << "TRA";
    break;
  case level::deb:
    p_out << "DEB";
    break;
  case level::inf:
    p_out << "INF";
    break;
  case level::war:
    p_out << "WAR";
    break;
  case level::err:
    p_out << "ERR";
    break;
  case level::fat:
    p_out << "FAT";
    break;
  default:
    p_out << "***";
  }

  return p_out;
}

} // namespace tnct::log::dat

#endif
