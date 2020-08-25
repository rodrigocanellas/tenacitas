#ifndef TENACITAS_CONCURRENT_WRITER_H
#define TENACITAS_CONCURRENT_WRITER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 07/2020

#include <cstdint>

#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief writer_t writes to a stream
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \endcode
template <typename t_log> struct writer_t {

  /// \brief writes a block to a stream, blocking the caller
  ///
  /// \tparam t_stream type of stream
  /// it must implement
  /// \code
  /// status::result operator()(const char * p_data, size_t p_size)
  /// \endcode
  ///
  /// \param p_stream is the \p t_stream object
  ///
  /// \param p_data is the begining of the buffer to be written
  ///
  /// \param p_size is the size of the buffer to be written
  ///
  /// \return status::ok on success, any other value on failure
  template <typename t_stream>
  status::result write_block(t_stream &p_stream, const char *p_data,
                             size_t p_size) noexcept {
    try {
      return p_stream(p_data, p_size);
    } catch (const std::exception &_ex) {
      concurrent_error(m_log, _ex.what());
      return concurrent::error_writing;
    }
  }

private:
  t_log m_log{"concurrent::writer"};
};

} // namespace concurrent
} // namespace tenacitas

#endif
