#ifndef TENACITAS_EVENTS_H
#define TENACITAS_EVENTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>

/// \brief master namespace
namespace tenacitas {

/// \brief events used in \p tenacitas namespaces by namespace \p
/// tenacitas::async
namespace event {

/// \brief Application can exit in a gracefully way
struct exit_app {

    /// \brief Output operator
    friend std::ostream &operator<<(std::ostream &p_out, const exit_app &) {
        p_out << "exit_app";
        return p_out;
    }
};

template <typename t_data>
struct new_data {

    new_data() = default;

    new_data(t_data &&p_data)
        : data(std::move(p_data)) {}

    friend std::ostream &operator<<(std::ostream &p_out, const new_data &) {
        p_out << "new_data";
        return p_out;
    }

    t_data data;
};

struct done_reading {
    friend std::ostream &operator<<(std::ostream &p_out, const done_reading &) {
        p_out << "done_reading ";
        return p_out;
    }
};

} // namespace event
} // namespace tenacitas

#endif
