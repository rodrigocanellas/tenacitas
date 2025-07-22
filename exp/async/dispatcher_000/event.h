/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_EXP_ASYNC_DISPATCHER_EVENT_H
#define TENACITAS_EXP_ASYNC_DISPATCHER_EVENT_H

#include <iostream>

namespace tenacitas::exp::async {

template <char t_id> struct event {
  friend std::ostream &operator<<(std::ostream &p_out, const event &) {
    p_out << "event " << t_id;
    return p_out;
  }

  static constexpr char id{t_id};
};

} // namespace tenacitas::exp::async

#endif
