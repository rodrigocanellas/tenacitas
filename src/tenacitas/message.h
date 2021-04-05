#ifndef TENACITAS_MESSAGE_H
#define TENACITAS_MESSAGE_H

#include <chrono>
#include <iostream>

namespace tenacitas {
namespace message {

/// \brief Application can exit in a gracefully wait
struct exit_app {
  friend std::ostream &operator<<(std::ostream &p_out, const exit_app &) {
    p_out << "exit_app";
    return p_out;
  }
};

/// \brief Application must stop now!
struct halt_app {
  friend std::ostream &operator<<(std::ostream &p_out, const halt_app &) {
    p_out << "halt_app";
    return p_out;
  }
};

} // namespace message
} // namespace tenacitas
#endif
