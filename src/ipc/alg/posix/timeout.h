#ifndef TENACITAS_LIB_IPC_POSIX_TIMEOUT_H
#define TENACITAS_LIB_IPC_POSIX_TIMEOUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <sys/select.h>

#include <chrono>

#include <tenacitas.lib/src/ipc/typ/types.h>
#include <tenacitas.lib/src/log/alg/logger.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::socket::alg {

/// \brief Checks if an operation on a socket is avaiable within a certain time
/// \return tenacitas::lib::socket::typ::status::ERROR_TIMEOUT if not
/// \return tenacitas::lib::socket::typ::status::ERROR_IO if there was an error
/// \return tenacitas::lib::socket::typ::status::OK if the operation is avaiable
template <typename t_time>
inline typ::status timeout(int p_socket, t_time p_time) {
  //  return typ::status::OK;
  if (p_time.count() == 0) {
    return typ::status::OK;
  }
  struct timeval _time {
    0, std::chrono::duration_cast<std::chrono::microseconds>(p_time).count()
  };

  fd_set _fd_set;
  FD_ZERO(&_fd_set);
  FD_SET(p_socket, &_fd_set);

  int _res = select(p_socket + 1, &_fd_set, NULL, NULL, &_time);

  switch (_res) {
  case 0:
    return typ::status::ERROR_TIMEOUT;
  case -1:
    TNCT_LOG_ERR(strerror(errno));
    return typ::status::ERROR_IO;
  default:
    return typ::status::OK;
  }
}

} // namespace tenacitas::lib::socket::alg

#endif
