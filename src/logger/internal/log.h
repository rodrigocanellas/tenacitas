#ifndef TENACITAS_LOGGER_BUS_LOG_H
#define TENACITAS_LOGGER_BUS_LOG_H

#include <chrono>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include <logger/level.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

/// \brief log is a class that guarantees thread safe writing to the log writer
///
/// \tparam t_specific_logger is the class that will implement a concrete log
///
class log {
public:
  /// \brief function responsible for actually writing the log message to a
  /// writer
  typedef std::function<void(std::string &&)> writer;

public:
  log() = delete;

  log(const log &p_log) : m_class(p_log.m_class) {}

  log(log &&p_log) noexcept : m_class(std::move(p_log.m_class)) {}

  log &operator=(const log &p_log) {
    if (this != &p_log) {
      m_class = p_log.m_class;
    }
    return *this;
  }

  log &operator=(log &&p_log) noexcept {
    if (this != &p_log) {
      m_class = std::move(p_log.m_class);
    }
    return *this;
  }

  virtual ~log() = default;

  /// \brief Constructor
  /// \param p_writer the actual writer. Please, read the \p log class
  /// documentation above to be aware of the methods \p t_writer should
  /// implement
  inline log(writer &&p_writer)
      : m_writer(std::move(p_writer)), m_is_writer_set(true) {}

  /// \brief sets the level of the log for 'this'
  inline void set_level(level p_level) { m_level = p_level; }

  /// \brief retrieves the level of the log for 'this'
  inline level get_level() const { return m_level; }

  /// \brief set_timestamp_as_number makes the timestamp to be printed as a
  /// number, instead of a string
  inline void set_timestamp_as_number(bool p_value = true) {
    m_timestamp_as_number = p_value;
  }

  /// \brief set_separator defines the separator to be used in the log
  /// messages
  ///
  /// \param p_separator the value of the separator
  inline void set_separator(char p_separator) { m_separator = p_separator; }

  /// \brief get_separator retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const { return m_separator; }

  //  inline void set_writer(writer &&p_writer) {
  //    if (!m_is_writer_set) {
  //      m_writer = std::move(p_writer);
  //      m_is_writer_set = true;
  //    }
  //  }

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
  template <typename... t_params>
  inline void debug(uint32_t p_line, const t_params &... p_params) {
    write(level::debug, p_line, p_params...);
  }

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
  template <typename... t_params>
  inline void info(uint32_t p_line, const t_params &... p_params) {
    write(level::info, p_line, p_params...);
  }

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
  template <typename... t_params>
  inline void warn(uint32_t p_line, const t_params &... p_params) {
    write(level::warn, p_line, p_params...);
  }

  /// \brief logs message with \p error severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  template <typename... t_params>
  inline void error(uint32_t p_line, const t_params &... p_params) {
    write(level::error, p_line, p_params...);
  }

  /// \brief logs message with \p fatal severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  template <typename... t_params>
  inline void fatal(uint32_t p_line, const t_params &... p_params) {
    write(level::fatal, p_line, p_params...);
  }

  //  bool is_writer_set() const { return m_is_writer_set; }
protected:
  log(std::string &&p_class, writer p_writer)
      : m_class(std::move(p_class)), m_writer(p_writer) {}

  log(const char *p_class, writer p_writer)
      : m_class(p_class), m_writer(p_writer) {}

private:
  /// \brief write will actually write the message
  ///
  /// tparam t_params are the types of the values to be logged, and each
  /// parameter must implement the \code std::ostream & operator<<(const t
  /// &)\endcode operator, where \p t is the type of the parameter
  ///
  /// \param p_level is the severity level of the message
  ///
  /// \param p_params are the values to be logged
  template <typename... t_params>
  void write(level p_level, uint32_t p_line, const t_params &... p_params) {

    if (can_log(p_level)) {
      std::ostringstream _stream;
      _stream << p_level << m_separator
              << (m_timestamp_as_number ? std::to_string(microseconds())
                                        : now())
              << m_separator << std::this_thread::get_id() << m_separator
              << m_class << m_separator << p_line;
      format(_stream, m_separator, p_params...);
      _stream << std::endl;
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_writer(_stream.str());
    }
  }

  //  const char *file_name(const char *p_file_path, char *p_file_name) {
  //    int16_t _len = static_cast<int16_t>(std::strlen(p_file_path));
  //    for (int16_t _i = _len - 1; _i > -1; --_i) {
  //      if ((p_file_path[_i] == '/') || (p_file_path[_i] == '\\')) {
  //        if (_i > _MAX_PATH) {
  //          return p_file_path;
  //        }
  //        std::memcpy(p_file_name, p_file_path + _i + 1, _len - _i);
  //        return p_file_name;
  //      }
  //    }
  //    return p_file_path;
  //  }

  /// \brief can_log informs if a message with a certain log level can be
  /// printed, considering the current log level
  ///
  /// \param p_level is the value of the level to be compared to the current
  /// log level
  ///
  /// \return \p true if the messsage can be logged, \p false otherwise
  ///
  bool can_log(level p_level) const;

  /// \brief format compile time recursion to solve the variadic template
  /// parameter
  template <typename t, typename... ts>
  inline void format(std::ostringstream &p_stream, const t &p_t,
                     const ts &... p_ts) const {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  /// \brief end of compile time recursion to solve the variadic template
  /// parameter
  template <typename t>
  inline void format(std::ostringstream &p_stream, const t &p_t) const {
    p_stream << p_t;
  }

  inline void format(std::ostringstream &p_stream, const double &p_t) const {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  inline void format(std::ostringstream &p_stream, const float &p_t) const {
    p_stream << std::setprecision(std::numeric_limits<float>::max_digits10)
             << p_t;
  }

  inline void format(std::ostringstream &p_stream, const bool &p_t) const {
    p_stream << (p_t ? "true" : "false");
  }

  inline uint64_t microseconds() const {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  inline uint64_t milliseconds() const {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  template <typename... t>
  inline void format(std::ostringstream &p_stream,
                     const std::tuple<t...> &p_t) {
    p_stream << to_str(p_t);
  }

  /// \brief copies a tuple fields into a string
  template <class TupType, size_t... I>
  std::string to_str(const TupType &_tup, std::index_sequence<I...>) {
    std::stringstream _stream;
    _stream << "(";
    (..., (_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
    _stream << ")";
    return _stream.str();
  }

  /// \brief copies one tuple field into a string
  template <class... T> std::string to_str(const std::tuple<T...> &_tup) {
    return to_str(_tup, std::make_index_sequence<sizeof...(T)>());
  }

  std::string now() const;

private:
  std::string m_class = {"no-class"};

  /// \brief m_writer is the writer where the messages will be logged.
  ///
  /// Please, read the \p log class documentation above to be aware of the
  /// methods \p t_writer should implement
  writer m_writer = {[](std::string &&) {
    std::cerr << "NO WRITER CONFIGURED FOR LOGGING" << std::endl;
  }};

  /// \brief m_mutex allows a thread safe writing to the log writer
  std::mutex m_mutex;

  /// \brief m_level is the current log level
  level m_level{get_level()};

  /// \brief m_separator is used to separate parts of the log message
  char m_separator{'|'};

  bool m_is_writer_set{false};

  bool m_timestamp_as_number{false};
};

} // namespace logger
} // namespace tenacitas

#endif // LOGGER_H
