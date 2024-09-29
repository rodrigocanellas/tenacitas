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
  UNIDENTIFIED,
  HANDLING_EXISTS,
  HANDLING_DOES_NOT_EXIST,
  HANDLER_USED,
  ZERO_AMOUNT,
  HANDLINGS_NOT_FOUND
};

static inline std::ostream &operator<<(std::ostream &p_out,
                                       const result &p_result) {
  switch (p_result) {
  case result::OK:
    p_out << "OK";
    break;
  case result::UNIDENTIFIED:
    p_out << "unidentified";
    break;
  case result::HANDLING_EXISTS:
    p_out << "handling exists";
    break;
  case result::HANDLING_DOES_NOT_EXIST:
    p_out << "handling does not exist";
    break;
  case result::HANDLER_USED:
    p_out << "handler used";
    break;
  case result::ZERO_AMOUNT:
    p_out << "zero amount";
    break;
  case result::HANDLINGS_NOT_FOUND:
    p_out << "handlings not found";
    break;
  }
  return p_out;
}

} // namespace tenacitas::lib::async

#endif
