#ifndef TENACITAS_LOGGER_BUSINESS_LOG_H
#define TENACITAS_LOGGER_BUSINESS_LOG_H

#include <functional>
#include <iomanip>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include <calendar/business/epoch.h>
#include <logger/business/internal/level.h>
#include <type/business/enum.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

///
/// \brief log is a template class that guarantees thread safe writing to the
/// log writer
///
class log
{
public:
  /// \brief function responsible for acually writing the log message to a
  /// writer
  typedef std::function<void(std::string&&)> writer;

public:
  ///
  /// \brief configure defines the writer that will be used to log messages
  /// \param p_writer the actual writer. Please, read the \p log class
  /// documentation above to be aware of the methods \p t_writer should
  /// implement
  ///
  inline static void configure(writer&& p_writer)
  {
    m_writer = std::move(p_writer);
  }

  log() = delete;
  log(const log&) = delete;
  log(log&&) noexcept = delete;
  log& operator=(const log&) = delete;
  log& operator=(log&&) noexcept = delete;
  ~log() = default;

  ///
  /// \brief set_test defines the log level to 'test'
  ///
  inline static void set_test() { m_level = level::test; }

  ///
  /// \brief set_debug defines the log level to 'debug'
  ///
  inline static void set_debug() { m_level = level::debug; }

  ///
  /// \brief set_info defines the log level to 'info'
  ///
  inline static void set_info() { m_level = level::info; }

  ///
  /// \brief set_warn defines the log level to 'warn'
  ///
  inline static void set_warn() { m_level = level::warn; }

  ///
  /// \brief set_error defines the log level to 'error'
  ///
  inline static void set_error() { m_level = level::error; }

  ///
  /// \brief set_separator defines the separator to be used in the log
  /// messages
  ///
  /// \param p_separator the value of the separator
  ///
  inline static void set_separator(char p_separator)
  {
    m_separator = p_separator;
  }

  ///
  /// \brief get_separator retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  ///
  inline static char get_separator() { return m_separator; }

  ///
  /// \brief logs message with \p test severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is \p level::test
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void test(t_str p_class,
                          t_int p_line,
                          const t_params&... p_params)
  {
    write(level::test, p_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p debug severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is \p level::debug
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void debug(t_str p_class,
                           t_int p_line,
                           const t_params&... p_params)
  {
    write(level::debug, p_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p info severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::info
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void info(t_str p_class,
                          t_int p_line,
                          const t_params&... p_params)
  {
    write(level::info, p_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p warn severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::warn
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void warn(t_str p_class,
                          t_int p_line,
                          const t_params&... p_params)
  {
    write(level::warn, p_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p error severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void error(t_str p_class,
                           t_int p_line,
                           const t_params&... p_params)
  {
    write(level::error, p_class, p_line, p_params...);
  }

  ///
  /// \brief logs message with \p fatal severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  ///
  template<typename t_str, typename t_int, typename... t_params>
  inline static void fatal(t_str p_class,
                           t_int p_line,
                           const t_params&... p_params)
  {
    write(level::fatal, p_class, p_line, p_params...);
  }

private:
  ///
  /// \brief write will actually write the message
  ///
  /// \tparam t_params are the types of the values to be logged, and each
  /// parameter must implement the \code std::ostream & operator<<(const t
  /// &)\endcode operator, where \p t is the type of the parameter
  ///
  /// \param p_level is the severity level of the message
  ///
  /// \param p_params are the values to be logged
  ///
  template<typename t_str, typename t_int, typename... t_params>
  static void write(level p_level,
                    t_str p_class,
                    t_int p_line,
                    const t_params&... p_params)
  {
    if (can_log(p_level)) {
      std::ostringstream _stream;
      //            _stream << type::business::e2t(p_level) << m_separator
      _stream << "" << level2str(p_level) << m_separator
              << tenacitas::calendar::business::epoch::millisecs()
              << m_separator << std::this_thread::get_id() << m_separator
              << p_class << m_separator << p_line;
      format(_stream, m_separator, p_params...);
      _stream << std::endl;
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_writer(_stream.str());
    }
  }

  ///
  /// \brief can_log informs if a message with a certain log level can be
  /// printed, considering the current log level
  ///
  /// \param p_level is the value of the level to be compared to the current
  /// log level
  ///
  /// \return \p true if the messsage can be logged, \p false otherwise
  ///
  static bool can_log(level p_level)
  {
    if (m_level == level::no_log) {
      return false;
    }
    //    if (p_level == level::test) {
    //      return true;
    //    }
    if ((p_level == level::error) || (p_level == level::fatal)) {
      return true;
    }
    return (p_level >= m_level);
  }

  ///
  /// \brief format compile time recursion to solve the variadic template
  /// parameter
  ///
  template<typename t, typename... ts>
  inline static void format(std::ostringstream& p_stream,
                            const t& p_t,
                            const ts&... p_ts)
  {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  ///
  /// \brief end of compile time recursion to solve the variadic template
  /// parameter
  ///
  template<typename t>
  inline void static format(std::ostringstream& p_stream, const t& p_t)
  {
    p_stream << p_t;
  }

  inline void static format(std::ostringstream& p_stream, const double& p_t)
  {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  inline void static format(std::ostringstream& p_stream, const float& p_t)
  {
    p_stream << std::setprecision(std::numeric_limits<float>::max_digits10)
             << p_t;
  }

private:
  ///
  /// \brief m_writer is the writer where the messages will be logged. Please,
  /// read the \p log class documentation above to be aware of the methods \p
  /// t_writer should implement
  ///
  static writer m_writer;

  ///
  /// \brief m_mutex allows a thread safe writing to the log writer
  ///
  static std::mutex m_mutex;

  ///
  /// \brief m_level is the current log level
  ///
  static level m_level;

  ///
  /// \brief m_separator is used to separate parts of the log message
  ///
  static char m_separator;
};

} // namespace logger
} // namespace business
} // namespace tenacitas

#endif // LOGGER_H
