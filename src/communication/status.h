#ifndef TENACITAS_COMMUNICATION_STATUS_H
#define TENACITAS_COMMUNICATION_STATUS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstdint>
#include <iostream>
#include <sstream>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

struct status {
  status() = delete;
  status(const status &) = default;
  status(status &&) = default;

  status &operator=(const status &) = default;
  status &operator=(status &&) = default;

  static const status ok;
  static const status error_unspecified;
  static const status error_connecting;
  static const status error_sending;
  static const status error_posting;
  static const status error_notifying;
  static const status error_creating_security;
  static const status error_receiving;
  static const status error_timeout;
  static const status end_of_message;

  inline friend std::ostringstream &operator<<(std::ostringstream &p_out,
                                               const status &p_status) {
    p_out << p_status.m_value;
    return p_out;
  }

  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const status &p_status) {
    p_out << p_status.m_value;
    return p_out;
  }

  constexpr bool operator==(const status &p_status) const {
    return m_value == p_status.m_value;
  }

  constexpr bool operator!=(const status &p_status) const {
    return m_value != p_status.m_value;
  }

private:
  inline status(uint16_t p_value) : m_value(p_value) {}

private:
  uint16_t m_value;
};

} // namespace communication
} // namespace tenacitas

#endif // STATUS_H
