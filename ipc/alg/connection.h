#ifndef TENACITAS_LIB_IPC_BUS_CONNECTION_H
#define TENACITAS_LIB_IPC_BUS_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>
#include <utility>

#include <tenacitas/lib/socket/typ/types.h>

namespace tenacitas::lib::socket::alg {

struct connection {
  using ptr = std::shared_ptr<connection>;
  virtual std::pair<typ::status, typ::buffer> read() = 0;
  virtual typ::status write(const std::byte *p_begin,
                            const std::byte *p_end) = 0;
};
} // namespace tenacitas::lib::socket::alg

#endif
