#ifndef TENACITAS_CONCURRENT_READER_H
#define TENACITAS_CONCURRENT_READER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 07/2020

#include <array>
#include <chrono>
#include <cstdint>
#include <future>
#include <string>
#include <utility>

#include <concurrent/traits.h>
#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

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
/// \tparam t_buffer_size is the size of the buffer to send and receive
/// messages
template<typename t_logger, std::size_t t_buffer_size = 8 * 1024>
struct reader
{
  typedef std::array<char, t_buffer_size> buffer;

  /// \brief reads data from a stream, blocking the caller until the read
  /// finishes
  ///
  /// \tparam t_stream stream from where data will be read
  /// it must implement:
  /// <code>
  ///
  /// <\endcode>
  ///
  /// \param p_stream stream of data
  ///
  /// \return {false, {}} if no data was read; {true, buffer} if some data was
  /// read
  template<typename t_stream>
  std::pair<status::result, buffer> read_block(t_stream& p_stream);

  template<typename t_stream, typename t_time = std::chrono::milliseconds>
  std::pair<status::result, buffer> read_block(t_stream& p_stream,
                                               t_time p_timeout);

  template<typename t_stream>
  std::pair<status::result, std::string> read_all_block(t_stream& p_stream);

  template<typename t_stream, typename t_time = std::chrono::milliseconds>
  std::pair<status::result, std::string> read_all_block(t_stream& p_stream,
                                                        t_time p_timeout);

  template<typename t_stream>
  std::pair<status::result, buffer> read_unblock(t_stream& p_stream);

  template<typename t_stream, typename t_time = std::chrono::milliseconds>
  std::pair<status::result, buffer> read_unblock(t_stream& p_stream,
                                                 t_time p_timeout);

  template<typename t_stream>
  std::pair<status::result, std::string> read_all_unblock(t_stream& p_stream);

  template<typename t_stream, typename t_time = std::chrono::milliseconds>
  std::pair<status::result, std::string> read_all_unblock(t_stream& p_stream,
                                                          t_time p_timeout);

  /// \brief
  ///
  /// \tparam t_stream
  ///
  /// \tparam t_handler must implement
  /// \code void operator()(const buffer &) \endcode
  ///
  /// \param p_stream
  ///
  /// \param p_handler
  template<typename t_stream, typename t_handler>
  void read_async(t_stream& p_stream, t_handler p_handler);

  /// \brief
  ///
  /// \tparam t_stream
  ///
  /// \tparam t_handler must implement
  /// \code void operator()(std::string &&) \endcode
  ///
  /// \param p_stream
  ///
  /// \param p_handler
  template<typename t_stream, typename t_handler>
  void read_all_async(t_stream& p_stream, t_handler p_handler);
};

} // namespace concurrent
} // namespace tenacitas

#endif
