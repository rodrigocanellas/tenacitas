#ifndef TENACITAS_LOGGER_H
#define TENACITAS_LOGGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <thread>

#include <tenacitas/calendar.h>
#include <tenacitas/number.h>
#include <tenacitas/type.h>

/// \brief master namespace
namespace tenacitas {

/// \brief logger classes
namespace logger {

/// \brief The level enum defines the possible logger::log levels
enum class level : int8_t {
  no_log = -1,
  debug = 0,
  info = 1,
  warn = 2,
  error = 3,
  fatal = 4
};

inline std::ostream &operator<<(std::ostream &p_out, level p_level) {

  switch (p_level) {
  case level::debug:
    p_out << "DEB";
    break;
  case level::info:
    p_out << "INF";
    break;
  case level::warn:
    p_out << "WAR";
    break;
  case level::error:
    p_out << "ERR";
    break;
  case level::fatal:
    p_out << "FAT";
    break;
  default:
    p_out << "-N-";
  }
  return p_out;
}

inline const char *level2str(level p_level) {
  static const char *_deb = "DEB";
  static const char *_inf = "INF";
  static const char *_war = "WAR";
  static const char *_err = "ERR";
  static const char *_fat = "FAT";
  static const char *_n_ = "-N_";

  switch (p_level) {
  case level::debug:
    return _deb;
  case level::info:
    return _inf;
  case level::warn:
    return _war;
  case level::error:
    return _err;
  case level::fatal:
    return _fat;
  default:
    return _n_;
  }
}

level global_level{level::warn};

void set_debug_level() { global_level = level::debug; }
void set_info_level() { global_level = level::info; }
void set_warn_level() { global_level = level::warn; }

bool can_log(level p_level) { return p_level >= global_level; }

/// \brief log is a class that guarantees thread safe writing to the log writer
///
/// \tparam t_specific_logger is the class that will implement a concrete log
///
template <bool use = true> class log {
public:
  /// \brief function responsible for actually writing the log message to a
  /// writer
  typedef std::function<void(const char *)> writer;

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

  /// \brief sets 'this' log level for level::debug
  inline void set_debug_level() { m_level = level::debug; }

  /// \brief sets 'this' log level for level::info
  inline void set_info_level() { m_level = level::info; }

  /// \brief sets 'this' log level for level::warn
  inline void set_warn_level() { m_level = level::warn; }

  /// \brief set_separator defines the separator to be used in the log
  /// messages
  ///
  /// \param p_separator the value of the separator
  inline void set_separator(char p_separator) { m_separator = p_separator; }

  /// \brief get_separator retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const { return m_separator; }

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
  template <typename t_class, typename... t_params>
  inline void debug(t_class *p_this, const char *p_func, uint16_t p_line,
                    const t_params &... p_params) {
    write(level::debug, p_this, p_func, p_line, p_params...);
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
  template <typename t_class, typename... t_params>
  inline void info(t_class *p_this, const char *p_func, uint16_t p_line,
                   const t_params &... p_params) {
    write(level::info, p_this, p_func, p_line, p_params...);
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
  template <typename t_class, typename... t_params>
  inline void warn(t_class *p_this, const char *p_func, uint16_t p_line,
                   const t_params &... p_params) {
    write(level::warn, p_this, p_func, p_line, p_params...);
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
  template <typename t_class, typename... t_params>
  inline void error(t_class *p_this, const char *p_func, uint16_t p_line,
                    const t_params &... p_params) {
    write(level::error, p_this, p_func, p_line, p_params...);
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
  template <typename t_class, typename... t_params>
  inline void fatal(t_class *p_this, const char *p_func, uint16_t p_line,
                    const t_params &... p_params) {
    write(level::fatal, p_this, p_func, p_line, p_params...);
  }

protected:
  log(std::string &&p_class, writer p_writer)
      : m_class(p_class), m_writer(p_writer), m_level(logger::level::warn) {}

private:
  struct appender {
    appender(char p_separator) : m_separator(p_separator) {
      std::memset(m_line, '\0', m_line_size);
    }

    void operator()(const char *p_str, bool p_sep = true) {
      auto _size = std::strlen(p_str);
      if ((m_p1 + _size) > m_line_size) {
        std::cerr << "overflow in log buffer trying to write '" << p_str
                  << "', starting at " << m_p1 << " and ending at "
                  << m_p1 + _size << std::endl;
        m_p1 += _size;
        return;
      }
      std::copy(&p_str[0], &p_str[_size], &m_line[m_p1]);
      m_p1 += _size;
      if (p_sep) {
        m_line[m_p1] = m_separator;
        ++m_p1;
      }
    }

    const char *get() const { return m_line; }

  private:
    static const uint16_t m_line_size{10 * 1024};

    char m_separator;

    uint16_t m_p1{0};

    char m_line[m_line_size + 1];
  };

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
#ifdef TENACITAS_LOG
  template <typename t_class, typename... t_params>
  void write(level p_level, t_class *p_this, const char *p_func,
             uint16_t p_line, const t_params &... p_params) {

    if (can_log(p_level)) {
      appender _append(m_separator);

      const char *_level_str = level2str(p_level);
      _append(_level_str);

      {
        std::string _now = std::to_string(calendar::now<>::microsecs_num());
        _append(_now.c_str());
      }

      {
        std::ostringstream _stream;
        _stream << p_this;
        _append(_stream.str().c_str());
      }

      _append(m_class.c_str());
      _append(p_func);

      {
        std::ostringstream _stream;
        _stream << std::this_thread::get_id();
        _append(_stream.str().c_str());
      }

      _append(std::to_string(p_line).c_str());

      format(_append, p_params...);

      _append("\n", false);

      std::lock_guard<std::mutex> _lock(m_mutex);
      m_writer(_append.get());
    }
  }
#else
  template <typename t_class, typename... t_params>
  void write(level, t_class *, const char *, uint16_t, const t_params &...) {}

#endif

  /// \brief can_log informs if a message with a certain log level can be
  /// printed, considering the current log level
  ///
  /// \param p_level is the value of the level to be compared to the current
  /// log level
  ///
  /// \return \p true if the messsage can be logged, \p false otherwise
  ///
  inline bool can_log(level p_level) const {
    if (m_level == level::no_log) {
      return false;
    }
    if ((p_level == level::error) || (p_level == level::fatal)) {
      return true;
    }
    return ((p_level >= m_level) || (logger::can_log(p_level)));
  }

  /// \brief format compile time recursion to solve the variadic template
  /// parameter
  template <typename t, typename... ts>
  inline void format(appender &p_append, const t &p_t,
                     const ts &... p_ts) const {
    format(p_append, p_t);
    format(p_append, p_ts...);
  }

  /// \brief end of compile time recursion to solve the variadic template
  /// parameter
  template <typename t>
  inline void format(appender &p_append, const t &p_t) const {
    std::stringstream _stream;
    if constexpr (type::is_tuple<t>::value) {
      _stream << to_str(p_t);
    } else {
      _stream << p_t;
    }
    p_append(_stream.str().c_str(), false);
  }

  inline void format(appender &p_append, const double &p_t) const {
    std::stringstream _stream;
    _stream << std::setprecision(std::numeric_limits<double>::max_digits10)
            << p_t;
    p_append(_stream.str().c_str(), false);
  }

  inline void format(appender &p_append, const float &p_t) const {
    std::stringstream _stream;
    _stream << std::setprecision(std::numeric_limits<double>::max_digits10)
            << p_t;
    p_append(_stream.str().c_str(), false);
  }

  inline void format(appender &p_append, const bool &p_t) const {
    static const char *_true = "true";
    static const char *_false = "true";
    p_append((p_t ? _true : _false), false);
  }

  inline void format(appender &p_append, std::thread::id p_id) const {
    std::stringstream _stream;
    _stream << p_id;
    p_append(_stream.str().c_str(), false);
  }

  /// \brief copies a tuple fields into a string
  template <class TupType, size_t... I>
  std::string to_str(const TupType &_tup, std::index_sequence<I...>) const {
    std::stringstream _stream;
    _stream << "(";
    (..., (_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
    _stream << ")";
    return _stream.str();
  }

  /// \brief copies one tuple field into a string
  template <class... T> std::string to_str(const std::tuple<T...> &_tup) const {
    return to_str(_tup, std::make_index_sequence<sizeof...(T)>());
  }

private:
  std::string m_class;

  /// \brief m_writer is the writer where the messages will be logged.
  ///
  /// Please, read the \p log class documentation above to be aware of the
  /// methods \p t_writer should implement
  writer m_writer = {[](std::string &&p_str) {
    std::stringstream _stream;
    _stream << "NO WRITER - " << p_str;
    std::cerr << _stream.str();
  }};

  /// \brief m_level is the current log level
  level m_level{logger::level::warn};

  /// \brief m_mutex allows a thread safe writing to the log writer
  std::mutex m_mutex;

  /// \brief m_separator is used to separate parts of the log message
  char m_separator{'|'};

  bool m_is_writer_set{false};
};

/// \brief logs message to \p std::cerr
template <bool use = true> struct cerr : public log<use> {
  inline explicit cerr(std::string &&p_class = "no-class")
      : log<use>(std::move(p_class),
                 [](const char *p_str) -> void { std::cerr << p_str; }) {}

  inline explicit cerr(const char *p_class)
      : log<use>(std::string(p_class),
                 [](const char *p_str) -> void { std::cerr << p_str; }) {}
};

/// \brief logs message to \p std::cout
template <bool use = true> struct cout : public log<use> {
  inline explicit cout(std::string &&p_class = "no-class")
      : log<use>(std::move(p_class),
                 [](const char *p_str) -> void { std::cout << p_str; }) {}

  inline explicit cout(const char *p_class)
      : log<use>(std::string(p_class),
                 [](const char *p_str) -> void { std::cout << p_str; }) {}
};

/// \brief The log struct logs message to \p std::clog
template <bool use = true> struct clog : public log<use> {
  inline explicit clog(std::string &&p_class = "no-class")
      : log<use>(std::move(p_class),
                 [](const char *p_str) -> void { std::clog << p_str; }) {}

  inline explicit clog(const char *p_class)
      : log<use>(std::string(p_class),
                 [](const char *p_str) -> void { std::clog << p_str; }) {}
};

} // namespace logger

} // namespace tenacitas

#endif
