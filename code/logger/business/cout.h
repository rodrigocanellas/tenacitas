#ifndef TENACITAS_LOGGER_BUS_COUT_H
#define TENACITAS_LOGGER_BUS_COUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <thread>

#include <calendar/business/epoch.h>
#include <logger/business/internal/configure_cout_log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the class group
namespace business {

///
/// \brief configure_cout_log configures logging to cout
///
/// This namespace function must be called only one time to configure logging to
/// \p cout
///
/// \details There are 6 log levels, from the lowest: 'test', 'debug', 'info',
/// 'warn', 'error' and 'fatal'. The log level starts in 'error', so only
/// 'error' and 'fatal' messages will be logged.
///
/// If one wants to change the log level, the 'cout_set_test', 'cout_set_debug',
/// 'cout_set_info' or 'cout_set_warn' must be called
///
/// A log line has this format:
/// log-level|timestamp-millisecs|thread-id|file|line|user-contents
///
/// For
/// T|1552098707355|140299908863808|thread_pool_tester.h|59|adding
/// (0000000005,012345678901234567890123456789,1552098707355)
///
void
configure_cout_log();

} // namespace business
} // namespace logger
} // namespace tenacitas

/// \brief cout_set_test sets the log level to 'test'. Aftet this call, all
/// the log messages from 'test' up will de written
#define cout_set_test()                                                        \
    tenacitas::logger::business::cout_log_t::instance().set_level(             \
      tenacitas::logger::business::level::test)

/// \brief cout_set_debug sets the log level to 'debug'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cout_set_debug()                                                       \
    tenacitas::logger::business::cout_log_t::instance().set_level(             \
      tenacitas::logger::business::level::debug)

/// \brief cout_set_info sets the log level to 'info'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cout_set_info()                                                        \
    tenacitas::logger::business::cout_log_t::instance().set_level(             \
      tenacitas::logger::business::level::info)

/// \brief cout_set_warn sets the log level to 'warn'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define cout_set_warn()                                                        \
    tenacitas::logger::business::cout_log_t::instance().set_level(             \
      tenacitas::logger::business::level::warn)

/// \brief cout_set_no_log makes no message to be logged to std::cout
#define cout_set_no_log()                                                      \
    tenacitas::logger::business::cout_log_t::instance().set_level(             \
      tenacitas::logger::business::level::no_log)

/// \brief cerr_debug allows to log with 'test' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// T|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cerr_debug("hello! ", 390)</tt> will generate
///
/// T|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_test(p_params...)                                                 \
    tenacitas::logger::business::cout_log_t::instance().test(                  \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

/// \brief cout_debug allows to log with 'debug' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// D|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cout_debug("hello! ", 390)</tt> will generate
///
/// D|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_debug(p_params...)                                                \
    tenacitas::logger::business::cout_log_t::instance().debug(                 \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

/// \brief cout_debug allows to log with 'info' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// I|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cout_info("hello! ", 390)</tt> will generate
///
/// I|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_info(p_params...)                                                 \
    tenacitas::logger::business::cout_log_t::instance().info(                  \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

/// \brief cout_debug allows to log with 'warn' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// W|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cout_warn("hello! ", 390)></tt> will generate
///
/// W|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_warn(p_params...)                                                 \
    tenacitas::logger::business::cout_log_t::instance().warn(                  \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

/// \brief cout_debug allows to log with 'error' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// E|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cout_error("hello! ", 390)</tt> will generate
///
/// E|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_error(p_params...)                                                \
    tenacitas::logger::business::cout_log_t::instance().error(                 \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

/// \brief cout_debug allows to log with 'fatal' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// F|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// For example <tt>cout_fatal("hello! ", 390)</tt> will generate
///
/// F|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define cout_fatal(p_params...)                                                \
    tenacitas::logger::business::cout_log_t::instance().fatal(                 \
      tenacitas::calendar::business::epoch::millisecs(),                       \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __FILE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      __LINE__,                                                                \
      tenacitas::logger::business::cout_log_t::instance().get_separator(),     \
      p_params)

#endif
