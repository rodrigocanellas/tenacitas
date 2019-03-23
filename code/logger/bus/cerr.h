#ifndef TENACITAS_LOGGER_CERR_H
#define TENACITAS_LOGGER_CERR_H

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com
/// \date nov/2018

#include <chrono>
#include <cstddef>
#include <thread>

#include <calendar/bus/epoch.h>
#include <logger/bus/internal/configure_cerr_log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace bus {

///
/// \brief configure_cerr_log configures logging to cerr
///
/// \details There are 5 log levels, from the lowest: 'debug', 'info', 'warn',
/// 'error' and 'fatal'. The log level starts in 'error', so only 'error' and
/// 'fatal' messages will be logged.
///
/// If one wants to change the log level, the 'cerr_set_debug', 'cerr_set_info'
/// or 'cerr_set_warn' macro command must be called
///
/// \code
//#include <logger/bus/cerr.h>
//
// int
// main(int argc, char **argv)
//{
//    tenacitas::logger::bus::configure_cerr_log();
//
//    cerr_set_debug();
//
//    cerr_debug("hello! ", 309);
//    cerr_debug("how are you doing? ", 3.14);
//    cerr_info("fine!! ", 'W');
//    cerr_info("and you?");
//    cerr_warn("great! got a new job!! ", 6987.58f);
//    cerr_warn("nice!! ", 10);
//}
/// \endcode
///
void
configure_cerr_log();

} // namespace bus
} // namespace logger
} // namespace tenacitas

/// \brief cerr_set_test sets the log level to 'test'. Aftet this call, only
/// log messages 'test' will de written
#define cerr_set_test()                                                        \
    tenacitas::logger::bus::cerr_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::test)

/// \brief cerr_set_debug sets the log level to 'debug'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cerr_set_debug()                                                       \
    tenacitas::logger::bus::cerr_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::debug)

/// \brief cerr_set_info sets the log level to 'info'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cerr_set_info()                                                        \
    tenacitas::logger::bus::cerr_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::info)

/// \brief cerr_set_warn sets the log level to 'warn'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cerr_set_warn()                                                        \
    tenacitas::logger::bus::cerr_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::warn)

/// \brief cerr_set_no_log makes no message to be logged to std::cerr
#define cerr_set_no_log()                                                      \
    tenacitas::logger::bus::cerr_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::no_log)

/// \brief cerr_debug allows to log with 'test' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// T|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_debug("hello! ", 390) will generate
///
/// T|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_test(p_params...)                                                 \
    tenacitas::logger::bus::cerr_log_t::instance().test(                       \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

/// \brief cerr_debug allows to log with 'debug' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// D|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_debug("hello! ", 390) will generate
///
/// D|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_debug(p_params...)                                                \
    tenacitas::logger::bus::cerr_log_t::instance().debug(                      \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

/// \brief cerr_debug allows to log with 'info' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// I|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_info("hello! ", 390) will generate
///
/// I|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_info(p_params...)                                                 \
    tenacitas::logger::bus::cerr_log_t::instance().info(                       \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

/// \brief cerr_debug allows to log with 'warn' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// W|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_warn("hello! ", 390) will generate
///
/// W|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_warn(p_params...)                                                 \
    tenacitas::logger::bus::cerr_log_t::instance().warn(                       \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

/// \brief cerr_debug allows to log with 'error' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// E|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_error("hello! ", 390) will generate
///
/// E|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_error(p_params...)                                                \
    tenacitas::logger::bus::cerr_log_t::instance().error(                      \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

/// \brief cerr_debug allows to log with 'fatal' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// F|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example cerr_fatal("hello! ", 390) will generate
///
/// F|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cerr_fatal(p_params...)                                                \
    tenacitas::logger::bus::cerr_log_t::instance().fatal(                      \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::cerr_log_t::instance().get_separator(),          \
      p_params)

#endif
