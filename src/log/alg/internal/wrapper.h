#ifndef TENACITAS_LIB_LOG_ALG_WRAPPER_H
#define TENACITAS_LIB_LOG_ALG_WRAPPER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#include <tenacitas.lib/src/log/typ/level.h>
#include <tenacitas.lib/src/number/alg/format.h>
#include <tenacitas.lib/src/traits/alg/traits.h>

/// \brief master namespace
namespace tenacitas::lib::log::alg {

/// \brief internal types, objects and functions
namespace internal {

static uint8_t max_file_name_lenght{15};

/// \brief Thread safe writing of log messages to console (cerr/cout/clog)
/// A log message has the format:
/// log-level|timestamp|file-name|thread-id|line-number|contents
///
/// \details \p log-level is TNCT_LOG_TRA, TNCT_LOG_DEB, TNCT_LOG_INF,
/// TNCT_LOG_WAR, TNCT_LOG_ERR or TNCT_LOG_FAT; \p timestamp is a EPOCH number
/// in microsecs; \p file-name is the name of the file where the message is
/// being writen; \p thread-id is the id of the thread that is logging; \p
/// line-number is the number of the line that is logging; contents are the log
/// message specific contents
template <bool use = true> class wrapper {
public:
  /// \brief Responsible for actually writing the log message
  typedef std::function<void(std::string &&)> writer;

public:
  /// \brief Default contructor not allowed
  wrapper() = default;

  /// \brief Copy constructor not allowed
  wrapper(const wrapper &) = delete;

  /// \brief Move constructor not allowed
  wrapper(wrapper &&) = delete;

  /// \brief Copy assignment not allowed
  wrapper &operator=(const wrapper &) = delete;

  /// \brief Move assignment not allowed
  wrapper &operator=(wrapper &&) = delete;

  /// \brief New operator not allowed
  void *operator new(size_t) = delete;

  /// \brief Delete operator not allowed
  void operator delete(void *) = delete;

  /// \brief Defines the function responsible for actually writing the message
  ///
  /// \tparam p_writer the function responsible for actually writing the
  /// message
  inline static void set_writer(writer p_writer) { m_writer = p_writer; }

  /// \brief Defines the separator to be used in the log messages
  /// Default is '|'
  ///
  /// \param p_separator the value of the separator
  inline static void set_separator(char p_separator) {
    m_separator = p_separator;
  }

  /// \brief Retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const { return m_separator; }

  /// \brief Logs message with \p test severity
  /// Each parameter must implement the output operator.
  /// The log message will only be allways, and it should only be used in
  /// tests
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void test(const char *p_file, uint16_t p_line,
                          t_params &&...p_params) {
    write(typ::level::test, p_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  /// \brief Logs message with \p trace severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// typ::level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void trace(const char *p_file, uint16_t p_line,
                           t_params &&...p_params) {
    if (typ::level::trace >= g_level) {
      write(typ::level::trace, p_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p debug severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// typ::level::debug or \p typ::level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void debug(const char *p_file, uint16_t p_line,
                           t_params &&...p_params) {
    if (typ::level::debug >= g_level) {
      write(typ::level::debug, p_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p info severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// typ::level::info, \p typ::level::debug or \p typ::level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void info(const char *p_file, uint16_t p_line,
                          t_params &&...p_params) {
    if (typ::level::info >= g_level) {
      write(typ::level::info, p_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p warn severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// typ::level::warn, \p typ::level::info, \p typ::level::debug or \p
  /// typ::level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void warn(const char *p_file, uint16_t p_line,
                          t_params &&...p_params) {
    if (typ::level::warn >= g_level) {
      write(typ::level::warn, p_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p error severity
  /// Each parameter must implement the output operator.
  /// \p typ::level::error messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void error(const char *p_file, uint16_t p_line,
                           t_params &&...p_params) {
    write(typ::level::error, p_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  /// \brief Logs message with \p fatal severity
  /// Each parameter must implement the output operator.
  /// \p typ::level::fatal messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void fatal(const char *p_file, uint16_t p_line,
                           t_params &&...p_params) {
    write(typ::level::fatal, p_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  inline static void set_trace() { g_level = typ::level::trace; }
  inline static void set_debug() { g_level = typ::level::debug; }
  inline static void set_info() { g_level = typ::level::info; }
  inline static void set_warn() { g_level = typ::level::warn; }

private:
  /// \brief Actually writes the message
  ///
  /// \tparam t_params are the types of the values to be logged, and each
  /// parameter must implement the output operator
  ///
  /// \param p_level is the severity level of the message
  ///
  /// \param p_file is the name of the file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  static void write(typ::level p_level, const char *p_file, uint16_t p_line,
                    t_params &&...p_params) {
    std::stringstream _stream;

    const double _now_us =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
    const time_t _now_s{static_cast<time_t>(_now_us / 1000000)};

    // YYYY-MM-DD HH:MM:SS
    char _time_str[4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 1];
    strftime(_time_str, sizeof _time_str, "%Y-%m-%d %H:%M:%S",
             localtime(&_now_s));

    _stream << p_level << m_separator << _time_str << ','
            << number::alg::format(
                   static_cast<uint32_t>(_now_us - (_now_s * 1000000)), 6, '0',
                   number::alg::align::right)
            << m_separator
            //            << _now_us - (_now_s * 1000000) << m_separator
            << std::this_thread::get_id() << m_separator << std::setfill(' ')
            << std::left << std::setw(max_file_name_lenght)
            << std::filesystem::path(p_file).filename().string() << m_separator
            << number::alg::format(p_line, uint8_t{5}) << m_separator;

    format(_stream, std::forward<t_params>(p_params)...);
    _stream << '\n';

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_writer(_stream.str());
  }

  /// \brief Compile time recursion to solve the variadic template parameter
  template <typename t, typename... ts>
  inline static void format(std::ostream &p_stream, const t &p_t,
                            const ts &...p_ts) {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  inline static void format(std::ostream &) {}

  inline static void format(std::ostream &p_stream, char p_c) {
    p_stream << p_c;
  }

  /// \brief End of compile time recursion to solve the variadic template
  /// parameter
  template <typename t>
  static void format(std::ostream &p_stream, const t &p_t) {
    if constexpr (traits::alg::is_tuple_v<std::decay_t<t>>) {
      format(p_stream, '(');
      format<0>(p_stream, p_t);
      format(p_stream, ')');
    } else {
      p_stream << p_t;
    }
  }

  /// \brief Appends a double to the buffer
  ///
  /// \param p_t is the double to be appended to the buffer
  static void format(std::ostream &p_stream, double p_t) {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  /// \brief Appends a float to the buffer
  ///
  /// \param p_t is the float to be appended to the buffer
  static void format(std::ostream &p_stream, float p_t) {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  /// \brief Appends a bool to the buffer
  ///
  /// \param p_t is the bool to be appended to the buffer
  static void format(std::ostream &p_stream, bool p_t) {
    constexpr char _true[]{"true"};
    constexpr char _false[]{"false"};
    p_stream << (p_t ? _true : _false);
  }

  /// \brief Appends a pair to the buffer
  ///
  /// \param t_first is the type of the first field of the pair
  ///
  /// \param t_second is the type of the second field of the pair
  ///
  /// \param p_pair is the pair to be appended to the buffer
  template <typename t_first, typename t_second>
  static void format(std::ostream &p_stream,
                     const std::pair<t_first, t_second> &p_pair) {
    p_stream << '(';
    format(p_stream, std::get<0>(p_pair));
    p_stream << ',';
    format(p_stream, std::get<1>(p_pair));
    p_stream << ')';
  }

  static void format(std::ostream &p_stream, std::chrono::minutes p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::seconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::milliseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::microseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::nanoseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::byte p_value) {
    p_stream << std::hex << std::uppercase << static_cast<uint16_t>(p_value);
  }

  //  /// \brief Copies a tuple fields into a string
  //  template <class TupType, size_t... I>
  //  static void to_str(std::ostream &p_stream, const TupType &_tup,
  //                     std::index_sequence<I...>) {
  //    //    std::stringstream _stream;
  //    p_stream << "(";
  //    (..., (p_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
  //    p_stream << ")";
  //    //    return _stream.str();
  //  }

  //  /// \brief Copies one tuple field into a string
  //  template <class... T>
  //  static void to_str(std::ostream &p_stream, const std::tuple<T...> &_tup) {
  //    to_str(p_stream, _tup, std::make_index_sequence<sizeof...(T)>());
  //  }

  template <size_t index = 0, typename... t_fields>
  static void format(std::ostream &p_stream,
                     const std::tuple<t_fields...> &p_fields) {
    format(p_stream, std::get<index>(p_fields));
    if constexpr (index + 1 != sizeof...(t_fields)) {
      format(p_stream, ',');
      format<index + 1>(p_stream, p_fields);
    }
  }

private:
  /// \brief Allows a thread safe writing to the log writer
  static std::mutex m_mutex;

  /// \brief Used to separate parts of the log message
  static char m_separator;

  /// \brief Function that actually writes the log message
  ///
  /// The default implementation writes the \p std::cerr
  static writer m_writer;

  /// \brief Current log level
  static typ::level g_level;
};

template <bool use> std::mutex wrapper<use>::m_mutex;

/// \brief Used to separate parts of the log message
template <bool use> char wrapper<use>::m_separator{'|'};

/// \brief Function that actually writes the log message
///
/// The default implementation writes the \p std::cerr
template <bool use>
typename wrapper<use>::writer wrapper<use>::m_writer{
    [](std::string &&p_str) -> void { std::cerr << p_str; }};

/// \brief Current log level
template <bool use> typ::level wrapper<use>::g_level{typ::level::warn};

} // namespace internal
} // namespace tenacitas::lib::log::alg
#endif
