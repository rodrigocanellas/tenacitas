#ifndef TENACITAS_LOGGER_BUS_LOG_H
#define TENACITAS_LOGGER_BUS_LOG_H

#include <chrono>
#include <functional>
#include <iomanip>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include <logger/_bus/level.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace _bus {

//
/// \brief log is a class that guarantees thread safe writing to the log writer
//

template <typename t_specific_logger> class log_t {
public:
  typedef t_specific_logger specific_logger;

  friend specific_logger;

  /// \brief function responsible for actually writing the log message to a
  // writer
  typedef std::function<void(std::string &&)> writer;

public:
  log_t() = default;
  log_t(const log_t &) = delete;
  log_t(log_t &&) noexcept = delete;
  log_t &operator=(const log_t &) = delete;
  log_t &operator=(log_t &&) noexcept = delete;
  ~log_t() = default;

private:
  //
  /// \brief Constructor
  // \param p_writer the actual writer. Please, read the \p log class
  // documentation above to be aware of the methods \p t_writer should
  // implement
  //
  inline log_t(writer &&p_writer) { m_writer = std::move(p_writer); }

  //
  /// \brief set_debug defines the log level to 'debug'
  //
  inline void set_debug() { m_level = level::debug; }

  //
  /// \brief set_info defines the log level to 'info'
  //
  inline void set_info() { m_level = level::info; }

  //
  /// \brief set_warn defines the log level to 'warn'
  //
  inline void set_warn() { m_level = level::warn; }

  //
  /// \brief set_error defines the log level to 'error'
  //
  inline void set_error() { m_level = level::error; }

  //
  /// \brief set_separator defines the separator to be used in the log
  // messages
  //
  // \param p_separator the value of the separator
  //
  inline void set_separator(char p_separator) { m_separator = p_separator; }

  //
  /// \brief get_separator retrieves the separator used in the log messages
  //
  // \return the value of the separator
  //
  inline char get_separator() { return m_separator; }

  inline void set_writer(writer &&p_writer) { m_writer = std::move(p_writer); }

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
  inline void debug(t_str p_class, t_int p_line, const t_params &... p_params) {
    write(level::debug, p_class, p_line, p_params...);
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
  inline void info(t_str p_class, t_int p_line, const t_params &... p_params) {
    write(level::info, p_class, p_line, p_params...);
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
  inline void warn(t_str p_class, t_int p_line, const t_params &... p_params) {
    write(level::warn, p_class, p_line, p_params...);
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
  inline void error(t_str p_class, t_int p_line, const t_params &... p_params) {
    write(level::error, p_class, p_line, p_params...);
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
  inline void fatal(t_str p_class, t_int p_line, const t_params &... p_params) {
    write(level::fatal, p_class, p_line, p_params...);
  }

private:
  //
  /// \brief write will actually write the message
  //
  // \tparam t_params are the types of the values to be logged, and each
  // parameter must implement the \code std::ostream & operator<<(const t
  // &)\endcode operator, where \p t is the type of the parameter
  //
  // \param p_level is the severity level of the message
  //
  // \param p_params are the values to be logged
  //
  template <typename t_str, typename t_int, typename... t_params>
  void write(level p_level, t_str p_class, t_int p_line,
             const t_params &... p_params) {
    if (can_log(p_level)) {
      std::ostringstream _stream;
      _stream << level2str(p_level) << m_separator << milliseconds()
              << m_separator << std::this_thread::get_id() << m_separator
              << p_class << m_separator << p_line;
      format(_stream, m_separator, p_params...);
      _stream << std::endl;
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_writer(_stream.str());
    }
  }

  //
  /// \brief can_log informs if a message with a certain log level can be
  // printed, considering the current log level
  //
  // \param p_level is the value of the level to be compared to the current
  // log level
  //
  // \return \p true if the messsage can be logged, \p false otherwise
  //
  bool can_log(level p_level) {
    if (m_level == level::no_log) {
      return false;
    }
    if ((p_level == level::error) || (p_level == level::fatal)) {
      return true;
    }
    return (p_level >= m_level);
  }

  //
  /// \brief format compile time recursion to solve the variadic template
  // parameter
  //
  template <typename t, typename... ts>
  inline void format(std::ostringstream &p_stream, const t &p_t,
                     const ts &... p_ts) {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  //
  /// \brief end of compile time recursion to solve the variadic template
  // parameter
  //
  template <typename t>
  inline void format(std::ostringstream &p_stream, const t &p_t) {
    p_stream << p_t;
  }

  inline void format(std::ostringstream &p_stream, const double &p_t) {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  inline void format(std::ostringstream &p_stream, const float &p_t) {
    p_stream << std::setprecision(std::numeric_limits<float>::max_digits10)
             << p_t;
  }

  uint64_t milliseconds() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

private:
  //
  /// \brief m_writer is the writer where the messages will be logged. Please,
  // read the \p log class documentation above to be aware of the methods \p
  // t_writer should implement
  //
  writer m_writer = {[](std::string &&) {}};

  //
  /// \brief m_mutex allows a thread safe writing to the log writer
  //
  std::mutex m_mutex;

  //
  /// \brief m_level is the current log level
  //
  level m_level = {level::info};

  //
  /// \brief m_separator is used to separate parts of the log message
  //
  char m_separator = {'|'};
};

// template <typename t_specific_logger>
// typename log_t<t_specific_logger>::writer log_t<t_specific_logger>::m_writer
// =
//    [](std::string &&) {};

// template <typename t_specific_logger>
// level log_t<t_specific_logger>::m_level = {level::error};

// template <typename t_specific_logger>
// char log_t<t_specific_logger>::m_separator = {'|'};

// template <typename t_specific_logger>
// std::mutex log_t<t_specific_logger>::m_mutex;

} // namespace _bus
} // namespace logger
} // namespace tenacitas

#endif // LOGGER_H
