#ifndef TENACITAS_LOGGER_H
#define TENACITAS_LOGGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <thread>

#ifdef TENACITAS_LOG

    #include <tenacitas.lib/calendar.h>
    #include <tenacitas.lib/number.h>
    #include <tenacitas.lib/type.h>

    /// \brief Wrapper to the debug log function
    #define TRA(p_params...)                                             \
        tenacitas::logger::internal::get_log().trace(__FILE__, __LINE__, \
                                                     p_params)

    /// \brief Wrapper to the debug log function
    #define DEB(p_params...)                                             \
        tenacitas::logger::internal::get_log().debug(__FILE__, __LINE__, \
                                                     p_params)

    /// \brief Wrapper to the info log function
    #define INF(p_params...)                                            \
        tenacitas::logger::internal::get_log().info(__FILE__, __LINE__, \
                                                    p_params)

    /// \brief Wrapper to the warn log function
    #define WAR(p_params...)                                            \
        tenacitas::logger::internal::get_log().warn(__FILE__, __LINE__, \
                                                    p_params)

    /// \brief Wrapper to the error log function
    #define ERR(p_params...)                                             \
        tenacitas::logger::internal::get_log().error(__FILE__, __LINE__, \
                                                     p_params)

    /// \brief Wrapper to the fatal log function
    #define FAT(p_params...)                                             \
        tenacitas::logger::internal::get_log().fatal(__FILE__, __LINE__, \
                                                     p_params)

    /// \brief Defines the character to separate the fields in a log message
    #define LOG_SEP(separator) \
        tenacitas::logger::internal::get_log().set_separator(separator)

/// \brief master namespace
namespace tenacitas {

/// \brief thread-safe log class, with log level definition and with functions
/// to control where the log is written
namespace logger {

/// \brief internal types, objects and functions
namespace internal {

/// \brief Defines the possible log levels
enum class level : int8_t {
    no_log = -1,
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5
};

/// \brief Output operator for \p level
///
/// \param p_out is the output stream
/// \param p_level is the level to be written to p_out
inline std::ostream &operator<<(std::ostream &p_out, level p_level) {

    switch (p_level) {
    case level::trace:
        p_out << "TRA";
        break;
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
        p_out << "***";
    }

    return p_out;
}

/// \brief Thread safe writing of log messages
/// A log message has the format:
/// log-level|timestamp|file-name|thread-id|line-number|contents
///
/// \details \p log-level is TRA, DEB, INF, WAR, ERR or FAT; \p timestamp is a
/// EPOCH number in microsecs; \p file-name is the name of the file where the
/// message is being writen; \p thread-id is the id of the thread that is
/// logging; \p line-number is the number of the line that is logging; contents
/// are the log message specific contents
class log {
public:
    /// \brief Responsible for actually writing the log message
    typedef std::function<void(std::string &&)> writer;

public:
    /// \brief Default contructor not allowed
    log() = default;

    /// \brief Copy constructor not allowed
    log(const log &) = delete;

    /// \brief Move constructor not allowed
    log(log &&) = delete;

    /// \brief Copy assignment not allowed
    log &operator=(const log &) = delete;

    /// \brief Move assignment not allowed
    log &operator=(log &&) = delete;

    /// \brief New operator not allowed
    void *operator new(size_t) = delete;

    /// \brief Delete operator not allowed
    void operator delete(void *) = delete;

    /// \brief Defines the function responsible for actually writing the message
    ///
    /// \tparam p_writer the function responsible for actually writing the
    /// message
    inline void set_writer(writer p_writer) { m_writer = p_writer; }

    /// \brief Defines the separator to be used in the log messages
    /// Default is '|'
    ///
    /// \param p_separator the value of the separator
    inline void set_separator(char p_separator) { m_separator = p_separator; }

    /// \brief Retrieves the separator used in the log messages
    ///
    /// \return the value of the separator
    inline char get_separator() const { return m_separator; }

    /// \brief Logs message with \p trace severity
    /// Each parameter must implement the output operator.
    /// The log message will only be printed if the current log level is \p
    /// level::trace
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    trace(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        if (level::trace >= g_level) {
            write(level::trace, p_file, p_line, p_params...);
        }
    }

    /// \brief Logs message with \p debug severity
    /// Each parameter must implement the output operator.
    /// The log message will only be printed if the current log level is \p
    /// level::debug or \p level::trace
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    debug(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        if (level::debug >= g_level) {
            write(level::debug, p_file, p_line, p_params...);
        }
    }

    /// \brief Logs message with \p info severity
    /// Each parameter must implement the output operator.
    /// The log message will only be printed if the current log level is \p
    /// level::info, \p level::debug or \p level::trace
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    info(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        if (level::info >= g_level) {
            write(level::info, p_file, p_line, p_params...);
        }
    }

    /// \brief Logs message with \p warn severity
    /// Each parameter must implement the output operator.
    /// The log message will only be printed if the current log level is \p
    /// level::warn, \p level::info, \p level::debug or \p level::trace
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    warn(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        if (level::warn >= g_level) {
            write(level::warn, p_file, p_line, p_params...);
        }
    }

    /// \brief Logs message with \p error severity
    /// Each parameter must implement the output operator.
    /// \p level::error messages are allways logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    error(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::error, p_file, p_line, p_params...);
    }

    /// \brief Logs message with \p fatal severity
    /// Each parameter must implement the output operator.
    /// \p level::fatal messages are allways logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the file that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    fatal(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::fatal, p_file, p_line, p_params...);
    }

    inline void set_trace() { g_level = level::trace; }
    inline void set_debug() { g_level = level::debug; }
    inline void set_info() { g_level = level::info; }
    inline void set_warn() { g_level = level::warn; }

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
    void write(level p_level,
               const char *p_file,
               uint16_t p_line,
               const t_params &... p_params) {

        std::stringstream _stream;

        _stream << p_level << m_separator << calendar::now<>::microsecs()
                << m_separator
                << std::filesystem::path(p_file).filename().string()
                << m_separator << std::this_thread::get_id() << m_separator
                << p_line << m_separator;
        format(_stream, p_params...);
        _stream << '\n';

        std::lock_guard<std::mutex> _lock(m_mutex);
        m_writer(_stream.str());
    }

    /// \brief Compile time recursion to solve the variadic template parameter
    template <typename t, typename... ts>
    inline void
    format(std::ostream &p_stream, const t &p_t, const ts &... p_ts) {
        format(p_stream, p_t);
        format(p_stream, p_ts...);
    }

    /// \brief End of compile time recursion to solve the variadic template
    /// parameter
    template <typename t>
    void format(std::ostream &p_stream, const t &p_t) {
        if constexpr (type::is_tuple<t>::value) {
            to_str(p_stream, p_t);
        } else {
            p_stream << p_t;
        }
    }

    /// \brief Appends a double to the buffer
    ///
    /// \param p_t is the double to be appended to the buffer
    void format(std::ostream &p_stream, const double &p_t) {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a float to the buffer
    ///
    /// \param p_t is the float to be appended to the buffer
    void format(std::ostream &p_stream, const float &p_t) {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a bool to the buffer
    ///
    /// \param p_t is the bool to be appended to the buffer
    void format(std::ostream &p_stream, const bool &p_t) {
        const char *_true = "true";
        const char *_false = "false";
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
    void format(std::ostream &p_stream,
                const std::pair<t_first, t_second> &p_pair) {
        p_stream << '(';
        format(p_stream, std::get<0>(p_pair));
        p_stream << ',';
        format(p_stream, std::get<1>(p_pair));
        p_stream << ')';
    }

    /// \brief Copies a tuple fields into a string
    template <class TupType, size_t... I>
    void to_str(std::ostream &p_stream,
                const TupType &_tup,
                std::index_sequence<I...>) {
        //    std::stringstream _stream;
        p_stream << "(";
        (..., (p_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
        p_stream << ")";
        //    return _stream.str();
    }

    /// \brief Copies one tuple field into a string
    template <class... T>
    void to_str(std::ostream &p_stream, const std::tuple<T...> &_tup) {
        to_str(p_stream, _tup, std::make_index_sequence<sizeof...(T)>());
    }

private:
    /// \brief Allows a thread safe writing to the log writer
    std::mutex m_mutex;

    /// \brief Used to separate parts of the log message
    char m_separator {'|'};

    /// \brief Function that actually writes the log message
    ///
    /// The default implementation writes the \p std::cerr
    writer m_writer {[](std::string &&p_str) -> void { std::cerr << p_str; }};

    /// \brief Current log level
    level g_level {level::warn};
};

/// \brief Definition of the globl log object

static log &get_log() {
    static log _logger;
    return _logger;
}

} // namespace internal

///// \brief Sets the global log level as 'trace'
inline void set_trace_level() { internal::get_log().set_trace(); }

/// \brief Sets the global log level as 'debug'
inline void set_debug_level() { internal::get_log().set_debug(); }

/// \brief Sets the global log level as 'info'
inline void set_info_level() { internal::get_log().set_info(); }

/// \brief Sets the global log level as 'warn'
inline void set_warn_level() { internal::get_log().set_warn(); }

/// \brief Defines that log messages will be written to \p std::cerr
inline void set_writer_cerr() {
    internal::get_log().set_writer(
        [](std::string &&p_str) -> void { std::cerr << p_str; });
}

/// \brief Defines that log messages will be written to \p std::cout
inline void set_writer_cout() {
    internal::get_log().set_writer(
        [](std::string &&p_str) -> void { std::cout << p_str; });
}

/// \brief Defines that log messages will be written to \p std::clog
inline void set_writer_clog() {
    internal::get_log().set_writer(
        [](std::string &&p_str) -> void { std::clog << p_str; });
}

/// \brief Defines the function used to write the log messages
inline void set_writer(std::function<void(std::string &&)> p_writer) {
    internal::get_log().set_writer(p_writer);
}

} // namespace logger
} // namespace tenacitas

#else

    // dummy log implementation

    /// \brief Wraps to the debug log function
    #define TRA(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Wraps to the debug log function
    #define DEB(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Wraps to the info log function
    #define INF(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Wraps to the warn log function
    #define WAR(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Wraps to the error log function
    #define ERR(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Wraps to the fatal log function
    #define FAT(p_params...) \
        { tenacitas::logger::dummy(std::cout, p_params); }

    /// \brief Defines the character to separate the fields in a log
    /// message
    #define LOG_SEP(separator) \
        {}

/// \brief master namespace
namespace tenacitas {

/// \brief thread-safe log class, with log level definition and with functions
/// to control where the log is written
namespace logger {

template <typename... t_params>
inline void dummy([[maybe_unused]] std::ostream &p_out,
                  [[maybe_unused]] t_params &&... p_params) {}

inline void set_trace_level() {}
inline void set_debug_level() {}
inline void set_info_level() {}
inline void set_warn_level() {}
inline void set_writer_cerr() {}
inline void set_writer_cout() {}
inline void set_writer_clog() {}

} // namespace logger
} // namespace tenacitas

#endif

#endif // TENACITAS_LOGGER_H
