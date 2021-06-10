#ifndef TENACITAS_ASYNC_TST_MSG_A_H
#define TENACITAS_ASYNC_TST_MSG_A_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/number.h>
#include <tenacitas.lib/type.h>

using namespace tenacitas;

typedef uint8_t event_id;
typedef uint16_t pool_num;
typedef uint16_t sub_id;
typedef uint16_t publish_id;
typedef uint32_t value;

// data message
template <event_id evt_id = 0>
struct event_t {

    static const event_id id {evt_id};

    explicit event_t(value p_value = 0)
        : m_counter(p_value)
        , m_up(up())
        , m_down(down())
        , m_d(2.5 * m_counter) {}

    event_t(const event_t &p_msg)
        : m_counter(p_msg.m_counter)
        , m_up(p_msg.m_up)
        , m_down(p_msg.m_down)
        , m_d(p_msg.m_d) {}

    event_t(event_t &&p_msg)
        : m_counter(p_msg.m_counter)
        , m_up(std::move(p_msg.m_up))
        , m_down(std::move(p_msg.m_down))
        , m_d(p_msg.m_d) {}

    event_t &operator=(const event_t &p_msg) {
        if (this != &p_msg) {
            m_counter = p_msg.m_counter;
            m_up = p_msg.m_up;
            m_down = p_msg.m_down;
            m_d = p_msg.m_d;
        }
        return *this;
    }

    event_t &operator=(event_t &&p_msg) {
        if (this != &p_msg) {
            m_counter = p_msg.m_counter;
            m_up = std::move(p_msg.m_up);
            m_down = std::move(p_msg.m_down);
            m_d = p_msg.m_d;
        }
        return *this;
    }

    inline bool operator==(const event_t &p_msg) const {
        return m_counter == p_msg.m_counter;
    }

    inline bool operator!=(const event_t &p_msg) const {
        return m_counter != p_msg.m_counter;
    }

    friend std::ostream &operator<<(std::ostream &p_out, const event_t &p_msg) {
        p_out << "[" << static_cast<uint16_t>(id) << ","
              << number::format<value>(p_msg.m_counter) << "," << p_msg.m_up
              << "," << p_msg.m_down << "," << p_msg.m_d << "]";
        return p_out;
    }

    inline value get_value() const { return m_counter; }

    event_t &operator++() {
        ++m_counter;
        m_up = up();
        m_down = down();
        m_d = 2.5 * m_counter;
        return *this;
    }

    event_t &operator++(int) {
        m_counter++;
        m_up = up();
        m_down = down();
        m_d = 2.5 * m_counter;
        return *this;
    }

private:
    inline std::string up() {
        return number::format<value>(std::numeric_limits<value>::min() +
                                     m_counter);
    }

    inline std::string down() {
        return number::format<value>(std::numeric_limits<value>::max() -
                                     m_counter);
    }

private:
    value m_counter {0};
    std::string m_up;
    std::string m_down;
    double m_d;
};

#endif // TENACITAS_ASYNC_TST_MSG_A_H
