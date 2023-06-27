#ifndef TENACITAS_LIB_IPC_TYP_TYPES_H
#define TENACITAS_LIB_IPC_TYP_TYPES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <tenacitas/lib/async/typ/channel_id.h>
#include <tenacitas/lib/async/typ/connection_id.h>
#include <tenacitas/lib/async/typ/message.h>
#include <tenacitas/lib/number/typ/id.h>

namespace tenacitas::lib::socket::typ {

enum class link : uint8_t { UNIX, IPV4, IPV6, BLUETOOTH };

enum class flow : uint8_t { STREAM, DGRAM };

// enum class boundary : uint8_t { LIMITED, UNLIMITED };

// enum class dynamic : uint8_t { OPEN_CLOSE, KEEP_OPENED };

enum class message_size : uint8_t { FIXED = 0, DELIMITED = 1 };

enum class status : uint8_t {
  OK,
  PARTIAL_READ,
  CONNECTION_CLOSED,
  ERROR_NOT_CONNECTED,
  ERROR_TIMEOUT_READING,
  ERROR_TIMEOUT_WRITING,
  ERROR_READING,
  ERROR_WRITING,
  ERROR_MAX_READING,
  ERROR_TIMEOUT,
  ERROR_IO,
  ERROR_INVALID_BUFFER,
  ERROR_UNDEFINED_INTERNAL_MESSAGE,
  ERROR_CATASTROPHIC
};

std::ostream &operator<<(std::ostream &p_out, status p_status) {
  switch (p_status) {
  case status::OK:
    p_out << "ok";
    break;
  case status::PARTIAL_READ:
    p_out << "partial_read";
    break;
  case status::CONNECTION_CLOSED:
    p_out << "connection_closed";
    break;
  case status::ERROR_NOT_CONNECTED:
    p_out << "error_not_connected";
    break;
  case status::ERROR_TIMEOUT_READING:
    p_out << "error_timeout_reading";
    break;
  case status::ERROR_TIMEOUT_WRITING:
    p_out << "error_timeout_writing";
    break;
  case status::ERROR_READING:
    p_out << "error_reading";
    break;
  case status::ERROR_WRITING:
    p_out << "error_writing";
    break;
  case status::ERROR_MAX_READING:
    p_out << "error_max_reading";
    break;
  case status::ERROR_TIMEOUT:
    p_out << "error_timeout";
    break;
  case status::ERROR_IO:
    p_out << "error io";
    break;
  case status::ERROR_INVALID_BUFFER:
    p_out << "error_invalid_buffer";
    break;
  case status::ERROR_UNDEFINED_INTERNAL_MESSAGE:
    p_out << "error_undefined_internal_message";
    break;
  case status ::ERROR_CATASTROPHIC:
    p_out << "error_catastrophic";
    break;
  }
  return p_out;
}

using read = std::function<std::pair<typ::status, ssize_t>(
    std::byte *, size_t, std::chrono::milliseconds)>;

struct endpoint {

  explicit endpoint(std::string_view p_ip, uint16_t p_port)
      : ip(p_ip), port(static_cast<uint16_t>(p_port)) {}

  endpoint() = default;
  endpoint(const endpoint &) = default;
  endpoint(endpoint &&) = default;
  endpoint &operator=(const endpoint &) = default;
  endpoint &operator=(endpoint &&) = default;

  bool operator==(const endpoint &p_endpoint) const {
    return (ip == p_endpoint.ip) && (port == p_endpoint.port);
  }

  bool operator!=(const endpoint &p_endpoint) const {
    return (ip != p_endpoint.ip) || (port != p_endpoint.port);
  }

  bool operator<(const endpoint &p_endpoint) const {
    if (ip < p_endpoint.ip) {
      return true;
    }
    if (ip > p_endpoint.ip) {
      return false;
    }
    if (port < p_endpoint.port) {
      return true;
    }
    return false;
  }

  std::string ip;
  uint16_t port{0};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const endpoint &p_endpoint) {
    p_out << '[' << p_endpoint.ip << ',' << p_endpoint.port << ']';
    return p_out;
  }
};

enum class internal_message_type : uint8_t { undefined = 0, subscribe = 1 };

internal_message_type
from_value(std::underlying_type<internal_message_type>::type p_value) {
  if (p_value == 1) {
    return internal_message_type::subscribe;
  }

  return internal_message_type::undefined;
}

template <message_size t_read_type, size_t t_buffer_size> struct read_traits;

template <size_t t_buffer_size>
struct read_traits<message_size::FIXED, t_buffer_size> {
  using buffer = std::array<std::byte, t_buffer_size>;
};

template <size_t t_buffer_size>
struct read_traits<message_size::DELIMITED, t_buffer_size> {
  using buffer = std::vector<std::byte>;
};

// template <size_t t_buffer_size>
// struct read_traits<message_type::multiple, t_buffer_size> {
//  using buffer = typ::buffer<t_buffer_size>;
//};

} // namespace tenacitas::lib::socket::typ

#endif
