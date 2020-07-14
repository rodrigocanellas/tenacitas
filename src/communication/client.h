#ifndef TENACITAS_COMMUNICATION_CLIENT_H
#define TENACITAS_COMMUNICATION_CLIENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <communication/internal/log.h>
#include <communication/internal/timeout.h>
//#include <communication/status.h>
#include <concurrent/loop.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

typedef std::pair<const char *, const char *> data;

/// \brief client_t is a client for remote communication
///
/// \tparam t_logger must provide
/// \code
///
/// template <typename ...p_params>
/// static void debug(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void info(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void warn(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void error(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void fatal(const char *, int, params...)
///
/// \endcode
///
/// \param
///
/// \tparam t_receive_buffer_size is the size of the buffer to send and receive
/// messages
template <typename t_logger, size_t t_receive_buffer_size = 8 * 1024>
struct client_t {

  /// \brief alias for the logger
  typedef t_logger logger;

  typedef concurrent::work_status work_status;

  typedef std::function<work_status(data &&)> sender;

  typedef std::function<std::pair<bool, data>()> receiver;

  client_t(sender &&p_sender, receiver &&p_receiver)
      : m_sender(p_sender), m_receiver(p_receiver) {}

  template <typename t_timeout>
  void send_block(const char *p_begin, const char *p_end, t_timeout p_timeout) {
    typedef concurrent::loop_t<data, logger> loop;
    loop _loop(m_sender, p_timeout,
               []() -> work_status { return work_status::dont_stop; },
               [p_begin, p_end]() -> data {
                 return {p_begin, p_end};
               });
    _loop.start();
  }

private:
  sender m_sender;
  receiver m_receiver;
};

} // namespace communication
} // namespace tenacitas

#endif
