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

#include <tenacitas/calendar.h>
#include <tenacitas/number.h>
#include <tenacitas/type.h>

/// \brief master namespace
namespace tenacitas {

/// \brief logger classes
namespace logger {

// dummy log implementation
#ifndef TENACITAS_LOG

void set_debug_level() {}
void set_info_level() {}
void set_warn_level() {}
void set_writer_cerr() {}
void set_writer_cout() {}
void set_writer_clog() {}

    /// \brief Wraps to the debug log function
    #define DEB(p_params...) \
        {}

    /// \brief Wraps to the info log function
    #define INF(p_params...) \
        {}

    /// \brief Wraps to the warn log function
    #define WAR(p_params...) \
        {}

    /// \brief Wraps to the error log function
    #define ERR(p_params...) \
        {}

    /// \brief Wraps to the fatal log function
    #define FAT(p_params...) \
        {}

    /// \brief Defines the character to separate the fields in a log message
    #define LOG_SEP(separator) \
        {}

#else

    /// \brief Wrapper to the debug log function
    #define DEB(p_params...)                                            \
        tenacitas::logger::internal::g_logger.debug(__FILE__, __LINE__, \
                                                    p_params)

    /// \brief Wrapper to the info log function
    #define INF(p_params...) \
        tenacitas::logger::internal::g_logger.info(__FILE__, __LINE__, p_params)

    /// \brief Wrapper to the warn log function
    #define WAR(p_params...) \
        tenacitas::logger::internal::g_logger.warn(__FILE__, __LINE__, p_params)

    /// \brief Wrapper to the error log function
    #define ERR(p_params...)                                            \
        tenacitas::logger::internal::g_logger.error(__FILE__, __LINE__, \
                                                    p_params)

    /// \brief Wrapper to the fatal log function
    #define FAT(p_params...)                                            \
        tenacitas::logger::internal::g_logger.fatal(__FILE__, __LINE__, \
                                                    p_params)

    /// \brief Defines the character to separate the fields in a log message
    #define LOG_SEP(separator) \
        tenacitas::logger::internal::g_logger.set_separator(separator)

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

/// \brief Translates a \p level into a const char *
inline const char *level2str(level p_level) {
    static const char *_tra = "TRA";
    static const char *_deb = "DEB";
    static const char *_inf = "INF";
    static const char *_war = "WAR";
    static const char *_err = "ERR";
    static const char *_fat = "FAT";
    static const char *_n_ = "***";

    switch (p_level) {
    case level::trace:
        return _tra;
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

/// \brief Output operator for \p level
inline std::ostream &operator<<(std::ostream &p_out, level p_level) {
    p_out << level2str(p_level);
    return p_out;
}

/// \brief Guarantees thread safe writing of log messages
/// A log message has the format:
/// log-level|timestamp|class-name|function-name|thread-id|line-number|contents
///
/// \details log-level is DEB, INF, WAR, ERR or FAT; timestamp is a EPOCH number
/// in microsecs; class-name is the name of the class which is loggin;
/// function-name is the name of the function that is logging; thread-id is the
/// id of the thread that is logging; line-number is the number of the line
/// where the log is being written; contents are the log message specific
/// contents
///
/// \tparam use exists to make \p log to be compiled only it is really used

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
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// The log message will only be printed if the current log level is \p
    /// level::debug \tparam t_params are the types of the values to be logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    trace(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::trace, p_file, p_line, p_params...);
    }

    /// \brief Logs message with \p debug severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// The log message will only be printed if the current log level is \p
    /// level::debug \tparam t_params are the types of the values to be logged
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    debug(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::debug, p_file, p_line, p_params...);
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
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    info(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::info, p_file, p_line, p_params...);
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
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    warn(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::warn, p_file, p_line, p_params...);
    }

    /// \brief Logs message with \p error severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// Error log messages are allways written.
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    error(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::error, p_file, p_line, p_params...);
    }

    /// \brief Llogs message with \p fatal severity
    /// Each parameter must implement the \code std::ostream &
    /// operator<<(const t &)\endcode operator, where \p t is the type of the
    /// parameter.
    /// Fatal log messages are allways written.
    ///
    /// \tparam t_params are the types of the values to be logged.
    ///
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    inline void
    fatal(const char *p_file, uint16_t p_line, const t_params &... p_params) {
        write(level::fatal, p_file, p_line, p_params...);
    }

private:
    /// \brief Actually writes the message
    ///
    /// \tparam t_params are the types of the values to be logged, and each
    /// parameter must implement the \code std::ostream & operator<<(const t
    /// &)\endcode operator, where \p t is the type of the parameter
    ///
    /// \param p_level is the severity level of the message
    ///
    /// \param p_file is the name of the function that is logging
    ///
    /// \param p_line is the line where the logging is done
    ///
    /// \param p_params are the contens of the log message
    template <typename... t_params>
    void write(level p_level,
               const char *p_file,
               uint16_t p_line,
               const t_params &... p_params) {
        if (p_level >= g_level) {

            std::stringstream _stream;

            _stream << level2str(p_level) << m_separator
                    << calendar::now<>::microsecs() << m_separator
                    << std::filesystem::path(p_file).filename().string()
                    << m_separator << std::this_thread::get_id() << m_separator
                    << p_line << m_separator;
            format(_stream, p_params...);
            _stream << "\n";

            std::lock_guard<std::mutex> _lock(m_mutex);
            m_writer(_stream.str());
        }
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
    /// \param p_append will append the double to the buffer
    ///
    /// \param p_t is the double to be appended to the buffer
    void format(std::ostream &p_stream, const double &p_t) {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a float to the buffer
    ///
    /// \param p_append will append the float to the buffer
    ///
    /// \param p_t is the float to be appended to the buffer
    void format(std::ostream &p_stream, const float &p_t) {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    /// \brief Appends a bool to the buffer
    ///
    /// \param p_append will append the bool to the buffer
    ///
    /// \param p_t is the bool to be appended to the buffer
    void format(std::ostream &p_stream, const bool &p_t) {
        const char *_true = "true";
        const char *_false = "true";
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
    template <typename t_first, typename t_second>
    void format(std::ostream &p_stream,
                const std::pair<t_first, t_second> &p_pair) {
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

public:
    /// \brief Global log level
    static level g_level;

private:
    /// \brief Allows a thread safe writing to the log writer
    std::mutex m_mutex;

    /// \brief Used to separate parts of the log message
    char m_separator {'|'};

    writer m_writer {[](std::string &&p_str) -> void { std::cerr << p_str; }};
};

level log::g_level {level::warn};

log g_logger;

} // namespace internal

/// \brief Sets the global log level as 'debug'
void set_debug_level() { internal::log::g_level = internal::level::debug; }

/// \brief Sets the global log level as 'info'
void set_info_level() { internal::log::g_level = internal::level::info; }

/// \brief Sets the global log level as 'warn'
void set_warn_level() { internal::log::g_level = internal::level::warn; }

void set_writer_cerr() {
    internal::g_logger.set_writer(
        [](std::string &&p_str) -> void { std::cerr << p_str; });
}

void set_writer_cout() {
    internal::g_logger.set_writer(
        [](std::string &&p_str) -> void { std::cout << p_str; });
}

void set_writer_clog() {
    internal::g_logger.set_writer(
        [](std::string &&p_str) -> void { std::clog << p_str; });
}
#endif

} // namespace logger

} // namespace tenacitas

#endif
