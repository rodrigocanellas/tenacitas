#ifndef TENACITAS_LOGGER_CERR_CERR_H
#define TENACITAS_LOGGER_CERR_CERR_H

// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
// at the root of \p tenacitas directory

// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

#include <logger/bus/t_log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace cerr {

struct log {

  //
  /// \brief set_debug defines the log level to 'debug'
  //
  inline static void set_debug() { m_log.set_debug(); }

  //
  /// \brief set_info defines the log level to 'info'
  //
  inline static void set_info() { m_log.set_info(); }

  //
  /// \brief set_warn defines the log level to 'warn'
  //
  inline static void set_warn() { m_log.set_warn(); }

  //
  /// \brief set_error defines the log level to 'error'
  //
  inline static void set_error() { m_log.set_error(); }

  //
  /// \brief set_separator defines the separator to be used in the log
  // messages
  //
  // \param p_separator the value of the separator
  //
  inline static void set_separator(char p_separator) {
    m_log.set_separator(p_separator);
  }

  //
  /// \brief logs message with \p debug severity
  //
  // \tparam t_params are the types of the values to be logged
  //
  // \attention each parameter must implement the \code std::ostream &
  // operator<<(const t &)\endcode operator, where \p t is the type of the
  // parameter
  //
  // \details the log message will only be printed if the current log level
  // is \p level::debug
  //
  template <typename t_str, typename t_int, typename... t_params>
  inline static void debug(t_str p_class, t_int p_line,
                           const t_params &... p_params) {
    m_log.debug(p_class, p_line, p_params...);
  }

  //
  /// \brief logs message with \p info severity
  //
  // \tparam t_params are the types of the values to be logged
  //
  // \attention each parameter must implement the \code std::ostream &
  // operator<<(const t &)\endcode operator, where \p t is the type of the
  // parameter
  //
  // \details the log message will only be printed if the current log level
  // is at least \p level::info
  //
  template <typename t_str, typename t_int, typename... t_params>
  static inline void info(t_str p_class, t_int p_line,
                          const t_params &... p_params) {
    m_log.info(p_class, p_line, p_params...);
  }

  //
  /// \brief logs message with \p warn severity
  //
  // \tparam t_params are the types of the values to be logged
  //
  // \attention each parameter must implement the \code std::ostream &
  // operator<<(const t &)\endcode operator, where \p t is the type of the
  // parameter
  //
  // \details the log message will only be printed if the current log level
  // is at least \p level::warn
  //
  template <typename t_str, typename t_int, typename... t_params>
  static inline void warn(t_str p_class, t_int p_line,
                          const t_params &... p_params) {
    m_log.warn(p_class, p_line, p_params...);
  }

  //
  /// \brief logs message with \p error severity
  //
  // \tparam t_params are the types of the values to be logged
  //
  // \attention each parameter must implement the \code std::ostream &
  // operator<<(const t &)\endcode operator, where \p t is the type of the
  // parameter
  //
  // \details the log message with this severity will always be printed
  //
  template <typename t_str, typename t_int, typename... t_params>
  static inline void error(t_str p_class, t_int p_line,
                           const t_params &... p_params) {
    m_log.error(p_class, p_line, p_params...);
  }

  //
  /// \brief logs message with \p fatal severity
  //
  // \tparam t_params are the types of the values to be logged
  //
  // \attention each parameter must implement the \code std::ostream &
  // operator<<(const t &)\endcode operator, where \p t is the type of the
  // parameter
  //
  // \details the log message with this severity will always be printed
  //
  template <typename t_str, typename t_int, typename... t_params>
  static inline void fatal(t_str p_class, t_int p_line,
                           const t_params &... p_params) {
    m_log.fatal(p_class, p_line, p_params...);
  }

private:
  static bus::t_log<log> m_log;
};

} // namespace cerr
} // namespace logger
} // namespace tenacitas

#endif
