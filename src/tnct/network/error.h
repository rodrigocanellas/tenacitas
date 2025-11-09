#ifndef TNCT_NETWORK_RESULT_H
#define TNCT_NETWORK_RESULT_H

#include <cstdint>
#include <iostream>

namespace tnct::network
{

enum class error : std::uint8_t
{
  // OK,
  // PARTIAL_READ,
  // CONNECTION_CLOSED,
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

}

std::ostream &operator<<(std::ostream &p_out, tnct::network::error p_status)
{
  using tnct::network::error;

  switch (p_status)
  {
  // case status::OK:
  //   p_out << "ok";
  //   break;
  // case status::PARTIAL_READ:
  //   p_out << "partial_read";
  //   break;
  // case status::CONNECTION_CLOSED:
  //   p_out << "connection_closed";
  //   break;
  case error::ERROR_NOT_CONNECTED:
    p_out << "error_not_connected";
    break;
  case error::ERROR_TIMEOUT_READING:
    p_out << "error_timeout_reading";
    break;
  case error::ERROR_TIMEOUT_WRITING:
    p_out << "error_timeout_writing";
    break;
  case error::ERROR_READING:
    p_out << "error_reading";
    break;
  case error::ERROR_WRITING:
    p_out << "error_writing";
    break;
  case error::ERROR_MAX_READING:
    p_out << "error_max_reading";
    break;
  case error::ERROR_TIMEOUT:
    p_out << "error_timeout";
    break;
  case error::ERROR_IO:
    p_out << "error io";
    break;
  case error::ERROR_INVALID_BUFFER:
    p_out << "error_invalid_buffer";
    break;
  case error::ERROR_UNDEFINED_INTERNAL_MESSAGE:
    p_out << "error_undefined_internal_message";
    break;
  case error ::ERROR_CATASTROPHIC:
    p_out << "error_catastrophic";
    break;
  }
  return p_out;
}

#endif
