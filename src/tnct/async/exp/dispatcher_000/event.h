/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_EXP_DISPATCHER_EVENT_H
#define TNCT_ASYNC_EXP_DISPATCHER_EVENT_H

#include <iostream>

namespace tnct::async::exp {

template <char t_id> struct event {
  friend std::ostream &operator<<(std::ostream &p_out, const event &) {
    p_out << "event " << t_id;
    return p_out;
  }

  static constexpr char id{t_id};
};

} // namespace tnct::async::exp

#endif
