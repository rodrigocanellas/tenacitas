#ifndef TENACITAS_LOGGER_BUS_LOG_H
#define TENACITAS_LOGGER_BUS_LOG_H

#include <iomanip>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <logger/bus/internal/level.h>
#include <type/bus/enum.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace bus {

///
/// \brief log is a template class that guarantees thread safe writing to the
/// log media
///
/// \tparam t_media is the actual media where the log messages will be written
/// to. It must implement:
/// - move constructor
/// - move assignment
/// - \code void operator()(std::string &&) \endcode operator
/// - \code static std::string type() \endcode method, which should return a
/// string identifying the media
///
/// \details log is a singleton, and the static method \p configure must be
/// called, only once, to define the actual media and before the first call to
/// any \p instance, or a a\p runtime_error exception will be thrown.
///
template<typename t_media>
class log
{
  public:
    ///
    /// \brief configure defines the media that will be used to log messages
    /// \param p_media the actual media. Please, read the \p log class
    /// documentation above to be aware of the methods \p t_media should
    /// implement
    ///
    static inline void configure(t_media&& p_media)
    {
        if (m_singleton == nullptr) {
            m_singleton = std::shared_ptr<log>(new log(std::move(p_media)));
        }
    }

    ///
    /// \brief instance returns the single instance of the \p log class
    /// \return the single instance of the \p log class
    ///
    /// \attention the \p configure method above should be called, only once,
    /// before the first call to \p instance, or a \p runtime_error exception
    /// will be thrown.
    ///
    static inline log& instance()
    {
        if (m_singleton == nullptr) {
            std::stringstream _stream;
            _stream << t_media::type()
                    << ": must call 'log::configure' before 'log::instance'";
            throw std::runtime_error(_stream.str());
        }
        return *m_singleton;
    }

    log() = delete;
    log(const log&) = delete;
    log(log&&) noexcept = delete;
    log& operator=(const log&) = delete;
    log& operator=(log&&) noexcept = delete;
    ~log() = default;

    ///
    /// \brief set_level defines the log level
    /// \param p_level
    ///
    inline void set_level(level p_level) { m_level = p_level; }

    ///
    /// \brief set_separator defines the separator to be used in the log
    /// messages
    ///
    /// \param p_separator the value of the separator
    ///
    inline void set_separator(char p_separator) { m_separator = p_separator; }

    ///
    /// \brief get_separator retrieves the separator used in the log messages
    ///
    /// \return the value of the separator
    ///
    inline char get_separator() { return m_separator; }

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
    template<typename... t_params>
    inline void test(const t_params&... p_params)
    {
        write(level::test, m_separator, p_params...);
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
    template<typename... t_params>
    inline void debug(const t_params&... p_params)
    {
        write(level::debug, m_separator, p_params...);
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
    template<typename... t_params>
    inline void info(const t_params&... p_params)
    {
        write(level::info, m_separator, p_params...);
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
    template<typename... t_params>
    inline void warn(const t_params&... p_params)
    {
        write(level::warn, m_separator, p_params...);
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
    template<typename... t_params>
    inline void error(const t_params&... p_params)
    {
        write(level::error, m_separator, p_params...);
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
    template<typename... t_params>
    inline void fatal(const t_params&... p_params)
    {
        write(level::fatal, m_separator, p_params...);
    }

  private:
    ///
    /// \brief log constructor
    /// \param p_media the actual media. Please, read the \p log class
    /// documentation above to be aware of the methods \p t_media should
    /// implement
    ///
    explicit log(t_media&& p_media)
      : m_media(std::move(p_media))
    {}

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
    template<typename... t_params>
    inline void write(level p_level, const t_params&... p_params)
    {
        if (can_log(p_level)) {
            std::ostringstream _stream;
            format(_stream, type::bus::e2t(p_level), p_params...);
            _stream << std::endl;
            std::lock_guard<std::mutex> _lock(m_mutex);
            m_media(_stream.str());
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
    inline bool can_log(level p_level) const
    {
        if (m_level == level::no_log) {
            return false;
        }
        if (p_level == level::test) {
            return true;
        }
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
    void static format(std::ostringstream& p_stream,
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
    void static format(std::ostringstream& p_stream, const t& p_t)
    {
        p_stream << p_t;
    }

    void static format(std::ostringstream& p_stream, const double& p_t)
    {
        p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
                 << p_t;
    }

    void static format(std::ostringstream& p_stream, const float& p_t)
    {
        p_stream << std::setprecision(std::numeric_limits<float>::max_digits10)
                 << p_t;
    }

  private:
    ///
    /// \brief m_media is the media where the messages will be logged. Please,
    /// read the \p log class documentation above to be aware of the methods \p
    /// t_media should implement
    ///
    t_media m_media;

    ///
    /// \brief m_mutex allows a thread safe writing to the log media
    ///
    std::mutex m_mutex;

    ///
    /// \brief m_level is the current log level
    ///
    level m_level = { level::error };

    ///
    /// \brief m_separator is used to separate parts of the log message
    ///
    char m_separator = { '|' };

    ///
    /// \brief m_singleton is the single instance of the log class
    ///
    static std::shared_ptr<log> m_singleton;
};

template<typename t_media>
typename std::shared_ptr<log<t_media>> log<t_media>::m_singleton;

} // namespace bus
} // namespace logger
} // namespace tenacitas

#endif // LOGGER_H
