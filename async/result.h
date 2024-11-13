/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_RESULT_H
#define TENACITAS_LIB_ASYNC_RESULT_H

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::async {

enum class result : std::uint8_t {
  HANDLING_FOUND,
  HANDLING_NOT_FOUND,
};

static inline std::ostream &operator<<(std::ostream &p_out,
                                       const result &p_result) {
  switch (p_result) {
  case result::HANDLING_FOUND:
    p_out << "handling exists";
    break;
  case result::HANDLING_NOT_FOUND:
    p_out << "handling does not exist";
    break;
  }
  return p_out;
}

} // namespace tenacitas::lib::async

#endif
