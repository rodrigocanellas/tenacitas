#ifndef TENACITAS_EVENTS_H
#define TENACITAS_EVENTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>

namespace tenacitas {
namespace event {

/// \brief Application can exit in a gracefully way
struct exit_app {

    /// \brief Output operator
    friend std::ostream &operator<<(std::ostream &p_out, const exit_app &) {
        p_out << "exit_app";
        return p_out;
    }
};

} // namespace event
} // namespace tenacitas
#endif
