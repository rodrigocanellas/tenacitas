/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_RESULT_H
#define TENACITAS_LIB_ASYNC_RESULT_H

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::async {

enum class result : std::uint8_t {
  OK = 0,
  HANDLING_FOUND,
  HANDLING_NOT_FOUND,
  ERRROR_PUBLISHNG,
  ERROR_CLEARING,
  ERROR_ADDING_HANDLER,
  ERROR_STOPPING
};

static inline std::ostream &operator<<(std::ostream &p_out, result p_result) {
  switch (p_result) {
  case result::OK:
    p_out << "ok";
    break;
  case result::HANDLING_FOUND:
    p_out << "handling exists";
    break;
  case result::HANDLING_NOT_FOUND:
    p_out << "handling does not exist";
    break;
  case result::ERRROR_PUBLISHNG:
    p_out << "error publishing";
    break;
  case result::ERROR_CLEARING:
    p_out << "error clearing";
    break;
  case result::ERROR_ADDING_HANDLER:
    p_out << "error adding handler";
    break;
  case result::ERROR_STOPPING:
    p_out << "error stopping";
    break;
  }
  return p_out;
}

} // namespace tenacitas::lib::async

#endif
