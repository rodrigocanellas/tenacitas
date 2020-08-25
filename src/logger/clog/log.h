#ifndef TENACITAS_LOGGER_CLOG_CLOG_H
#define TENACITAS_LOGGER_CLOG_CLOG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

#include <logger/internal/log_t.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the class group
namespace clog {

///
/// \brief The log struct logs message to \p std::log
///
struct log {

  inline explicit log(std::string &&m_class) : m_class(std::move(m_class)) {}
  inline explicit log(const char *m_class) : m_class(m_class) {}

  log() = delete;

  log(const log &p_log) : m_class(p_log.m_class) {}

  log(log &&p_log) : m_class(std::move(p_log.m_class)) {}

  log &operator=(const log &p_log) {
    if (this != &p_log) {
      m_class = p_log.m_class;
    }
    return *this;
  }

  log &operator=(log &&p_log) {
    if (this != &p_log) {
      m_class = std::move(p_log.m_class);
    }
    return *this;
  }

  ~log() = default;

  /// \brief set_debug defines the log level to 'debug'
  inline static void set_debug() { m_log.set_debug(); }

  ///
  /// \brief set_info defines the log level to 'info'
  ///
  inline static void set_info() { m_log.set_info(); }

  ///
  /// \brief set_warn defines the log level to 'warn'
  ///
  inline static void set_warn() { m_log.set_warn(); }

  ///
  /// \brief set_error defines the log level to 'error'
  ///
  inline static void set_error() { m_log.set_error(); }

  ///
  /// \brief set_separator defines the separator to be used in the log
  /// messages
  ///
  /// \param p_separator the value of the separator
  ///
  inline static void set_separator(char p_separator) {
    m_log.set_separator(p_separator);
  }

  /// \brief logs message with \p debug severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \param p_file is supposed to contain the file name where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \param p_line is supposed to contain the line number where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is \p level::debug
  ///
  template <typename... t_params>
  inline void debug(uint32_t p_line, const t_params &... p_params) {
    m_log.debug(m_class, p_line, p_params...);
  }

  /// \brief logs message with \p info severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  ///
  /// \param p_file is supposed to contain the file name where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \param p_line is supposed to contain the line number where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::info
  ///
  template <typename... t_params>
  inline void info(uint32_t p_line, const t_params &... p_params) {
    m_log.info(m_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p warn severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  ///
  /// \param p_file is supposed to contain the file name where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \param p_line is supposed to contain the line number where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::warn
  ///
  template <typename... t_params>
  inline void warn(uint32_t p_line, const t_params &... p_params) {
    m_log.warn(m_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p error severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///

  ///
  /// \param p_file is supposed to contain the file name where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \param p_line is supposed to contain the line number where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  ///
  template <typename... t_params>
  inline void error(uint32_t p_line, const t_params &... p_params) {
    m_log.error(m_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p fatal severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///

  ///
  /// \param p_file is supposed to contain the file name where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \param p_line is supposed to contain the line number where the logging is
  /// occurring, but the user can actually pass any value here
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  ///
  template <typename... t_params>
  inline void fatal(uint32_t p_line, const t_params &... p_params) {
    m_log.fatal(m_class, p_line, p_params...);
  }

private:
  static log_t<log> m_log;
  std::string m_class = {"no-class"};
};

} // namespace clog
} // namespace logger
} // namespace tenacitas

#endif
