#ifndef TENACITAS_LIB_IPC_WRITER_H
#define TENACITAS_LIB_IPC_WRITER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.c

#include <array>
#include <chrono>
#include <cstddef>
#include <vector>

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/ipc/evt/events.h>
#include <tenacitas.lib/src/ipc/typ/types.h>
#include <tenacitas.lib/src/log/alg/logger.h>

namespace tenacitas::lib::socket::alg {

struct writer {
  writer() = default;

  template <typename t_timeout = std::chrono::milliseconds>
  writer(const t_timeout &p_timeout)
      : m_timeout(std::chrono::duration_cast<decltype(m_timeout)>(p_timeout)) {}

  template <typename t_connection>
  typ::status operator()(t_connection &p_connection, const std::byte *p_begin,
                         const std::byte *p_end) {
    return {p_connection.write(p_begin, p_end, m_timeout)};
  }

  template <typename t_connection>
  typ::status operator()(t_connection &p_connection, std::string &&p_buffer) {
    const std::byte *_begin{reinterpret_cast<const std::byte *>(&p_buffer[0])};
    const std::byte *_end{
        reinterpret_cast<const std::byte *>(&p_buffer[p_buffer.size()])};
    return (*this)(p_connection, _begin, _end);
  }

  template <typename t_connection>
  inline typ::status operator()(t_connection &p_connection,
                                std::vector<std::byte> &&p_buffer) {
    return (*this)(p_connection, &p_buffer[0], &p_buffer[p_buffer.size()]);
  }

private:
  std::chrono::microseconds m_timeout{0ms};
};
} // namespace tenacitas::lib::socket::alg

#endif
