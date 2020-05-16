#ifndef TENACITAS_LOGGER_FILE_LOG_H
#define TENACITAS_LOGGER_FILE_LOG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include <logger/file/internal/file_controller.h>
#include <logger/file/internal/file_writer.h>
#include <logger/internal/log_t.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the class group
namespace file {

///
/// \brief The log struct logs message to a file
///
struct log {

  ///
  /// \brief configure configures logging in file
  ///
  /// \param p_path path is where the log files will be generated
  ///
  /// \param p_base_name first string in the log file name; one can use, for
  /// example, the name of the program
  ///
  /// \param p_max_file_size maximum size of the file, in bytes, before a new
  /// one is created
  ///
  /// \param p_retention amount of time that the file will exist, before being
  /// deleted, in minutes
  ///
  /// \param p_closed_extension is a string that will be appended to the file
  /// name, showing that it was closed, because it has reached its maximum size,
  /// or the program has finished.
  ///
  /// \details Using the default values, log files will be created at
  /// the current path; the name of the files will begin with 'log';
  /// when it reaches 10MB a new log file will be created; each
  /// log file will last up to 2 days
  ///
  /// A log file name is the combination of 'p_base_name' concatenated with the
  /// process id (PID), and with the amount of milliseconds of when the file was
  /// created, like: 'log_0000008165_1545843474201246.log'.
  ///
  /// When this files reaches (almost) 10MB, it will be renamed to
  /// 'log_0000008165_1545843474201246.log.closed'.
  ///
  /// When it is more that 2 days old, it will be deleted.
  ///
  /// \details There are 5 log levels, from the lowest: 'debug', 'info', 'warn',
  /// 'error' and 'fatal'. The log level starts in 'error', so only 'error' and
  /// 'fatal' messages will be logged.
  ///
  /// If one wants to change the log level, the 'file_set_test',
  /// 'file_set_debug', 'file_set_info' or 'file_set_warn' must be called
  ///
  /// A log line has this format:
  /// log-level|timestamp-millisecs|thread-id|file|line|user-contents
  ///
  /// For example
  /// DEB|1552098707355|140299908863808|thread_pool_tester.h|59|adding
  /// (0000000005,012345678901234567890123456789,1552098707355)
  ///
  static void configure(
      const std::string &p_path = ".", const std::string &p_base_name = "log",
      uint32_t p_max_file_size = 10 * 1024 * 1024,
      std::chrono::minutes p_retention = std::chrono::minutes(2 * 24 * 60),
      const std::string &p_closed_extension = "closed") {

    file_controller _file_controller(p_path, p_base_name, p_max_file_size,
                                     p_retention, p_closed_extension);

    typedef file_writer_t<file_controller> file_writer;
    typedef std::shared_ptr<file_writer> file_writer_ptr;

    file_writer_ptr _file_writer_ptr(
        std::make_shared<file_writer>(std::move(_file_controller)));

    m_log.set_writer([_file_writer_ptr](std::string &&p_str) -> void {
      (*_file_writer_ptr)(std::move(p_str));
    });
  }

  ///
  /// \brief set_debug defines the log level to 'debug'
  ///
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

  ///
  /// \brief logs message with \p debug severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \tparam t_int is the type of number used for the \p p_line param
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
  template <typename t_str, typename t_int, typename... t_params>
  inline static void debug(t_str p_file, t_int p_line,
                           const t_params &... p_params) {
    m_log.debug(p_file, p_line, p_params...);
  }

  /// \brief logs message with \p info severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \tparam t_int is the type of number used for the \p p_line param
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
  template <typename t_str, typename t_int, typename... t_params>
  static inline void info(t_str p_file, t_int p_line,
                          const t_params &... p_params) {
    m_log.info(p_file, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p warn severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \tparam t_int is the type of number used for the \p p_line param
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
  template <typename t_str, typename t_int, typename... t_params>
  static inline void warn(t_str p_file, t_int p_line,
                          const t_params &... p_params) {
    m_log.warn(p_file, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p error severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \tparam t_int is the type of number used for the \p p_line param
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
  template <typename t_str, typename t_int, typename... t_params>
  static inline void error(t_str p_file, t_int p_line,
                           const t_params &... p_params) {
    m_log.error(p_file, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p fatal severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \tparam t_str is the type of string that is used for the \p p_file param
  ///
  /// \tparam t_int is the type of number used for the \p p_line param
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
  template <typename t_str, typename t_int, typename... t_params>
  static inline void fatal(t_str p_file, t_int p_line,
                           const t_params &... p_params) {
    m_log.fatal(p_file, p_line, p_params...);
  }

private:
  static log_t<log> m_log;
};

} // namespace file
} // namespace logger
} // namespace tenacitas

#endif /// FILE_H
