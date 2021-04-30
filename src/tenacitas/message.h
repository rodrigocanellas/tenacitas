#ifndef TENACITAS_MESSAGE_H
#define TENACITAS_MESSAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>

#include <tenacitas/number.h>

namespace tenacitas {
namespace message {

/// \brief Application can exit in a gracefully way
struct exit_app {

    /// \brief Output operator
    friend std::ostream &operator<<(std::ostream &p_out, const exit_app &) {
        p_out << "exit_app";
        return p_out;
    }
};

} // namespace message
} // namespace tenacitas
#endif
