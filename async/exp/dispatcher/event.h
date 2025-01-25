/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATHER_EVENT_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATHER_EVENT_H

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::async::exp {

template <char t_id> struct event {
  friend std::ostream &operator<<(std::ostream &p_out, const event &p_event) {
    p_out << "event " << t_id << " value " << p_event.value;
    return p_out;
  }
  uint32_t value{0};

  static constexpr char id{t_id};
};

} // namespace tenacitas::lib::async::exp

#endif
