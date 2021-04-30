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

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params...) p_log.debug(__func__, __LINE__, p_params)

/// \brief Wraps to the info log function
#define INF(p_log, p_params...) p_log.info(__func__, __LINE__, p_params)

/// \brief Wraps to the warn log function
#define WAR(p_log, p_params...) p_log.warn(__func__, __LINE__, p_params)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params...) p_log.error(__func__, __LINE__, p_params)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params...) p_log.fatal(__func__, __LINE__, p_params)

/// \brief master namespace
namespace tenacitas {

/// \brief logger classes
namespace logger {

// dummy log implementations
#ifndef TENACITAS_LOG

void set_debug_level() {}
void set_info_level() {}
void set_warn_level() {}

template <bool use = true> struct cerr {

    cerr(const char *) {}
    cerr(std::string &&) {}
    cerr(const std::string &) {}

    template <typename... t_params>
    void debug(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void info(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void warn(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void error(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void fatal(const char *, size_t, const t_params &...) {}

    inline void set_debug_level() {}

    inline void set_info_level() {}

    inline void set_warn_level() {}
};

template <bool use = true> struct cout {
    cout(const char *) {}
    cout(std::string &&) {}
    cout(const std::string &) {}

    template <typename... t_params>
    void debug(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void info(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void warn(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void error(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void fatal(const char *, size_t, const t_params &...) {}

    inline void set_debug_level() {}

    inline void set_info_level() {}

    inline void set_warn_level() {}
};

template <bool use = true> struct clog {
    clog(const char *) {}
    clog(std::string &&) {}
    clog(const std::string &) {}

    template <typename... t_params>
    void debug(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void info(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void warn(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void error(const char *, size_t, const t_params &...) {}

    template <typename... t_params>
    void fatal(const char *, size_t, const t_params &...) {}

    inline void set_debug_level() {}

    inline void set_info_level() {}

    inline void set_warn_level() {}
};

#else

namespace internal {

/// \brief Defines the possible log levels
enum class level : int8_t {
    no_log = -1,
    debug = 0,
    info = 1,
    warn = 2,
    error = 3,
    fatal = 4
};

/// \brief Output operator for \p level
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

/// \brief Translates a \p level into a const char *
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

/// \brief Global log level
level global_level{level::warn};

/// \brief Checks if a log is allowed, based on the global level
///
/// \brief p_level is the level which one wants to use for logging
bool can_log(internal::level p_level) {
    return p_level >= internal::global_level;
}

/// \brief Guarantees thread safe writing of log messages
/// A log message has the format:
/// log-level|timestamp|class-name|function-name|thread-id|line-number|contents
///
/// \details log-level is DEB, INF, WAR, ERR or FAT; timestamp is a EPOCH number
/// in microsecs; class-name is the name of the class which is loggin;
/// function-name is the name of the function that is logging; thread-id is the
/// id of the thread that is logging; line-number is the number of the line
/// where the log is beig written; contents are the log message specific
/// contents
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true> class log {
  public:
    /// \brief Responsible for actually writing the log message
    typedef std::function<void(std::string &&)> writer;

  public:
    /// \brief Default contructor not allowed
    log() = delete;

    /// \brief Copy constructor
    log(const log &p_log) : m_class(p_log.m_class) {}

    /// \brief Move constructor
    log(log &&p_log) noexcept : m_class(std::move(p_log.m_class)) {}

    /// \brief Copy assignment
    log &operator=(const log &p_log) {
        if (this != &p_log) {
            m_class = p_log.m_class;
        }
        return *this;
    }

    /// \brief Move assignment
    log &operator=(log &&p_log) noexcept {
        if (this != &p_log) {
            m_class = std::move(p_log.m_class);
        }
        return *this;
    }

    virtual ~log() = default;

    /// \brief Constructor
    /// \param p_writer the actual writer
    inline log(writer &&p_writer) : m_writer(std::move(p_writer)) {}

    /// \brief Sets 'this' log level for level::debug
    inline void set_debug_level() { m_level = level::debug; }

    /// \brief Sets 'this' log level for level::info
    inline void set_info_level() { m_level = level::info; }

    /// \brief Sets 'this' log level for level::warn
    inline void set_warn_level() { m_level = level::warn; }

    /// \brief Defines the separator to be used in the log messages
    /// Default is '|'
    ///
    /// \param p_separator the value of the separator
    inline void set_separator(char p_separator) { m_separator = p_separator; }

    /// \brief Retrieves the separator used in the log messages
    ///
    /// \return the value of the separator
    inline char get_separator() const { return m_separator; }

    /// \brief Logs message with \p debug severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// The log message will only be printed if the current log level is \p
    /// level::debug \tparam t_params are the types of the values to be logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void debug(const char *p_func, uint16_t p_line,
                      const t_params &...p_params) {
        write(level::debug, p_func, p_line, p_params...);
    }

    /// \brief Logs message with \p info severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// The log message will only be printed if the current log level is \p
    /// level::info \tparam t_params are the types of the values to be logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void info(const char *p_func, uint16_t p_line,
                     const t_params &...p_params) {
        write(level::info, p_func, p_line, p_params...);
    }

    /// \brief Logs message with \p warn severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// The log message will only be printed if the current log level is \p
    /// level::warn \tparam t_params are the types of the values to be logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void warn(const char *p_func, uint16_t p_line,
                     const t_params &...p_params) {
        write(level::warn, p_func, p_line, p_params...);
    }

    /// \brief Logs message with \p error severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// Error log messages are allways written.
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void error(const char *p_func, uint16_t p_line,
                      const t_params &...p_params) {
        write(level::error, p_func, p_line, p_params...);
    }

    /// \brief Llogs message with \p fatal severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// Fatal log messages are allways written.
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void fatal(const char *p_func, uint16_t p_line,
                      const t_params &...p_params) {
        write(level::fatal, p_func, p_line, p_params...);
    }

  protected:
    /// \brief Constructor
    ///
    /// \param p_class is an identifier of the class that is logging
    ///
    /// \param p_writer is the function that actually logs the message
    log(std::string &&p_class, writer p_writer)
        : m_class(p_class), m_writer(p_writer), m_level(level::warn) {}

  private:
    /// \brief Appends strings to a buffer for optimize writing log messages
    struct appender {
        /// \brief Constructor
        ///
        /// \param p__separator is the field separator in the log messages
        appender(char p_separator) : m_separator(p_separator) {
            std::memset(m_line, '\0', m_line_size);
        }

        /// \brief Appends a string to the internal buffer
        ///
        /// \param p_str is the string to be appended
        ///
        /// \param p_sep defines if the separator must be written after the \p
        /// p_str
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

        /// \brief Appends a char to the internal buffer
        ///
        /// \param p_char is the char to be appended
        ///
        /// \param p_sep defines if the separator must be written after the \p
        /// p_str
        void operator()(char p_char, bool p_sep = true) {
            auto _size = 1;
            if ((m_p1 + _size) > m_line_size) {
                std::cerr << "overflow in log buffer trying to write '"
                          << p_char << "', starting at " << m_p1
                          << " and ending at " << m_p1 + _size << std::endl;
                m_p1 += _size;
                return;
            }
            m_line[m_p1] = p_char;
            m_p1 += _size;
            if (p_sep) {
                m_line[m_p1] = m_separator;
                ++m_p1;
            }
        }

        /// \return The buffer to be logged
        const char *get() const { return m_line; }

      private:
        /// \brief Size of the buffer
        static const uint16_t m_line_size{50 * 1024};

        /// \brief Separator of the log fields
        char m_separator;

        /// \brief Index in the buffer
        uint16_t m_p1{0};

        /// \brief Buffer
        char m_line[m_line_size + 1];
    };

  private:
    /// \brief Actually writes the message
    ///
    /// \tparam t_params are the types of the values to be logged, and each
    /// parameter must implement the \code std::ostream & operator<<(const t
    /// &)\endcode operator, where \p t is the type of the parameter
    ///
    /// \param p_level is the severity level of the message
    ///
    /// \param p_func is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    void write(level p_level, const char *p_func, uint16_t p_line,
               const t_params &...p_params) {

        //    if (can_log(p_level)) {
        //      appender _append(m_separator);

        //      const char *_level_str = level2str(p_level);
        //      _append(_level_str);

        //      {
        //        std::string _now =
        //        std::to_string(calendar::now<>::microsecs());
        //        _append(_now.c_str());
        //      }

        //      _append(m_class.c_str());
        //      _append(p_func);

        //      {
        //        std::ostringstream _stream;
        //        _stream << std::this_thread::get_id();
        //        _append(_stream.str().c_str());
        //      }

        //      _append(std::to_string(p_line).c_str());

        //      format(_append, p_params...);

        //      _append("\n", false);

        //      std::lock_guard<std::mutex> _lock(m_mutex);
        //      m_writer(_append.get());
        //    }
        if (can_log(p_level)) {

            std::stringstream _stream;

            _stream << level2str(p_level) << m_separator
                    << calendar::now<>::microsecs() << m_separator << m_class
                    << m_separator << p_func << m_separator
                    << std::this_thread::get_id() << m_separator << p_line
                    << m_separator;
            format(_stream, p_params...);
            _stream << "\n";

            std::lock_guard<std::mutex> _lock(m_mutex);
            m_writer(_stream.str());
        }
    }

    /// \brief Informs if a message with a certain log level can be printed,
    /// considering the current log level
    ///
    /// \param p_level is the value of the level to be compared to the current
    /// log level
    ///
    /// \return \p true if the messsage can be logged, \p false otherwise
    bool can_log(level p_level) const {
        if (m_level == level::no_log) {
            return false;
        }
        if ((p_level == level::error) || (p_level == level::fatal)) {
            return true;
        }
        return ((p_level >= m_level) || (internal::can_log(p_level)));
    }

    /// \brief Compile time recursion to solve the variadic template parameter
    //  template <typename t, typename... ts>
    //  inline void format(appender &p_append, const t &p_t,
    //                     const ts &... p_ts) const {
    //    format(p_append, p_t);
    //    format(p_append, p_ts...);
    //  }

    template <typename t, typename... ts>
    inline void format(std::ostream &p_stream, const t &p_t,
                       const ts &...p_ts) const {
        format(p_stream, p_t);
        format(p_stream, p_ts...);
    }

    /// \brief End of compile time recursion to solve the variadic template
    /// parameter
    //  template <typename t> void format(appender &p_append, const t &p_t)
    //  const
    //  {
    //    std::stringstream _stream;
    //    if constexpr (type::is_tuple<t>::value) {
    //      _stream << to_str(p_t);
    //    } else {
    //      _stream << p_t;
    //    }
    //    p_append(_stream.str().c_str(), false);
    //  }

    template <typename t>
    void format(std::ostream &p_stream, const t &p_t) const {
        if constexpr (type::is_tuple<t>::value) {
            to_str(p_stream, p_t);
        } else {
            p_stream << p_t;
        }
    }

    /// \brief Appends a double to the buffer
    ///
    /// \param p_append will append the double to the buffer
    ///
    /// \param p_t is the double to be appended to the buffer
    //  void format(appender &p_append, const double &p_t) const {
    //    std::stringstream _stream;
    //    _stream <<
    //    std::setprecision(std::numeric_limits<double>::max_digits10)
    //            << p_t;
    //    p_append(_stream.str().c_str(), false);
    //  }

    void format(std::ostream &p_stream, const double &p_t) const {

        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a float to the buffer
    ///
    /// \param p_append will append the float to the buffer
    ///
    /// \param p_t is the float to be appended to the buffer
    //  void format(appender &p_append, const float &p_t) const {
    //    std::stringstream _stream;
    //    _stream <<
    //    std::setprecision(std::numeric_limits<double>::max_digits10)
    //            << p_t;
    //    p_append(_stream.str().c_str(), false);
    //  }

    void format(std::ostream &p_stream, const float &p_t) const {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a bool to the buffer
    ///
    /// \param p_append will append the bool to the buffer
    ///
    /// \param p_t is the bool to be appended to the buffer
    //  void format(appender &p_append, const bool &p_t) const {
    //    static const char *_true = "true";
    //    static const char *_false = "true";
    //    p_append((p_t ? _true : _false), false);
    //  }

    void format(std::ostream &p_stream, const bool &p_t) const {
        static const char *_true = "true";
        static const char *_false = "true";
        p_stream << (p_t ? _true : _false);
    }

    /// \brief Appends a pair to the buffer
    ///
    /// \param t_first is the type of the first field of the pair
    ///
    /// \param t_second is the type of the second field of the pair
    ///
    /// \param p_append will append the pair to the buffer
    ///
    /// \param p_pair is the pair to be appended to the buffer
    //  template <typename t_first, typename t_second>
    //  void format(appender &p_append,
    //              const std::pair<t_first, t_second> &p_pair) const {
    //    p_append('(', false);
    //    format(p_append, std::get<0>(p_pair));
    //    p_append(',', false);
    //    format(p_append, std::get<1>(p_pair));
    //    p_append(')', false);
    //  }

    template <typename t_first, typename t_second>
    void format(std::ostream &p_stream,
                const std::pair<t_first, t_second> &p_pair) const {
        p_stream << '(';
        format(p_stream, std::get<0>(p_pair));
        p_stream << ',';
        format(p_stream, std::get<1>(p_pair));
        p_stream << ')';
    }

    /// \brief Appends a thread id to the buffer
    ///
    /// \param p_append will append the thread id to the buffer
    ///
    /// \param p_id is the thread id to be appended to the buffer
    //  inline void format(appender &p_append, std::thread::id p_id) const {
    //    std::stringstream _stream;
    //    _stream << p_id;
    //    p_append(_stream.str().c_str(), false);
    //  }

    /// \brief Copies a tuple fields into a string
    template <class TupType, size_t... I>
    void to_str(std::ostream &p_stream, const TupType &_tup,
                std::index_sequence<I...>) const {
        //    std::stringstream _stream;
        p_stream << "(";
        (..., (p_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
        p_stream << ")";
        //    return _stream.str();
    }

    //  template <typename t_type>
    //  void format(std::ostream &p_stream, std::shared_ptr<t_type> p_ptr) {
    //    p_stream << (p_ptr ? *p_ptr : "nullptr");
    //  }

    //  template <typename t_type>
    //  void format(std::ostream &p_stream, std::unique_ptr<t_type> p_ptr) {
    //    p_stream << (p_ptr ? *p_ptr : "nullptr");
    //  }

    //  template <typename t_type>
    //  void format(std::ostream &p_stream, t_type *p_ptr) {
    //    p_stream << (p_ptr ? *p_ptr : "nullptr");
    //  }

    /// \brief Copies one tuple field into a string
    template <class... T>
    void to_str(std::ostream &p_stream, const std::tuple<T...> &_tup) const {
        to_str(p_stream, _tup, std::make_index_sequence<sizeof...(T)>());
    }

  private:
    /// \brief Name of the class responsible for logging
    std::string m_class;

    /// \brief Responsible for writing the log messages
    ///
    /// Please, read the \p log class documentation above to be aware of the
    /// methods \p t_writer should implement
    writer m_writer = {[](std::string &&p_str) {
        std::stringstream _stream;
        _stream << p_str;
        std::cerr << _stream.str();
    }};

    /// \brief Current log level
    level m_level{level::warn};

    /// \brief Allows a thread safe writing to the log writer
    std::mutex m_mutex;

    /// \brief Used to separate parts of the log message
    char m_separator{'|'};
};

} // namespace internal

/// \brief Sets the global log level as 'debug'
void set_debug_level() { internal::global_level = internal::level::debug; }

/// \brief Sets the global log level as 'info'
void set_info_level() { internal::global_level = internal::level::info; }

/// \brief Sets the global log level as 'warn'
void set_warn_level() { internal::global_level = internal::level::warn; }

/// \brief Logs message to \p std::cerr
/// A log message has the format:
/// log-level|timestamp|class-name|function-name|thread-id|line-number|contents
///
/// \details log-level is DEB, INF, WAR, ERR or FAT; timestamp is a EPOCH number
/// in microsecs; class-name is the name of the class which is loggin;
/// function-name is the name of the function that is logging; thread-id is the
/// id of the thread that is logging; line-number is the number of the line
/// where the log is beig written; contents are the log message specific
/// contents
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true> struct cerr : public internal::log<use> {
    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit cerr(std::string &&p_class = "no-class")
        : internal::log<use>(
              std::move(p_class),
              [](std::string &&p_str) -> void { std::cerr << p_str; }) {}

    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit cerr(const char *p_class)
        : internal::log<use>(
              std::string(p_class),
              [](std::string &&p_str) -> void { std::cerr << p_str; }) {}
};

/// \brief Logs message to \p std::cout
/// A log message has the format:
/// log-level|timestamp|class-name|function-name|thread-id|line-number|contents
///
/// \details log-level is DEB, INF, WAR, ERR or FAT; timestamp is a EPOCH number
/// in microsecs; class-name is the name of the class which is loggin;
/// function-name is the name of the function that is logging; thread-id is the
/// id of the thread that is logging; line-number is the number of the line
/// where the log is beig written; contents are the log message specific
/// contents
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true> struct cout : public internal::log<use> {

    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit cout(std::string &&p_class = "no-class")
        : internal::log<use>(
              std::move(p_class),
              [](std::string &&p_str) -> void { std::cout << p_str; }) {}

    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit cout(const char *p_class)
        : internal::log<use>(
              std::string(p_class),
              [](std::string &&p_str) -> void { std::cout << p_str; }) {}
};

/// \brief Logs message to \p std::clog
/// A log message has the format:
/// log-level|timestamp|class-name|function-name|thread-id|line-number|contents
///
/// \details log-level is DEB, INF, WAR, ERR or FAT; timestamp is a EPOCH number
/// in microsecs; class-name is the name of the class which is loggin;
/// function-name is the name of the function that is logging; thread-id is the
/// id of the thread that is logging; line-number is the number of the line
/// where the log is beig written; contents are the log message specific
/// contents
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true> struct clog : public internal::log<use> {

    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit clog(std::string &&p_class = "no-class")
        : internal::log<use>(
              std::move(p_class),
              [](std::string &&p_str) -> void { std::clog << p_str; }) {}

    /// \brief Constructor
    /// \param p_class is the name of the class responsible for logging
    inline explicit clog(const char *p_class)
        : internal::log<use>(
              std::string(p_class),
              [](std::string &&p_str) -> void { std::clog << p_str; }) {}
};

#endif

} // namespace logger

} // namespace tenacitas

#endif
