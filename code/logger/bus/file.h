#ifndef TENACITAS_LOGGER_BUS_FILE_H
#define TENACITAS_LOGGER_BUS_FILE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <thread>

#include <calendar/bus/epoch.h>
#include <logger/bus/internal/configure_file_log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the class group
namespace bus {

///
/// \brief configure_file_log configures logging in file
///
/// This namespace function must be called only one time to configure logging to
/// a file
///
/// \param p_path path is where the log files will be generated
/// \param p_base_name first string in the log file name; one can use, for
/// example, the name of the program
/// \param p_max_file_size maximum size of the file, in bytes
/// \param p_retention amount of time that the file will exist, before being
/// deleted, in minutes
/// \param p_closed_extension is a string that will be appended to the file
/// name, showing that it was closed, because it has reached its maximum size,
/// or the program has finished.
///
/// \details Using the default values, log files will be created at the current
/// path; the name of the files will begin with 'log'; when it reaches 10MB a
/// new log file will be created; each log file will last up to 2 days
///
/// A log file name is the combination of 'p_base_name' concatenated
/// with the process id (PID), and with the amount of milliseconds of when the
/// file was created, like: 'log_0000008165_1545843474201246.log'.
///
/// When this files reaches (almost) 10MB, it will be renamed to
/// 'log_0000008165_1545843474201246.log.closed'.
///
/// When it is more that 2 days old, it will be deleted.
///
/// \details There are 6 log levels, from the lowest: 'test', 'debug', 'info',
/// 'warn', 'error' and 'fatal'. The log level starts in 'error', so only
/// 'error' and 'fatal' messages will be logged.
///
/// If one wants to change the log level, the 'file_set_test', 'file_set_debug',
/// 'file_set_info' or 'file_set_warn' must be called
///
/// A log line has this format:
/// log-level|timestamp-millisecs|thread-id|file|line|user-contents
/// \example
/// T|1552098707355|140299908863808|thread_pool_tester.h|59|adding
/// (0000000005,012345678901234567890123456789,1552098707355)
///
void
configure_file_log(
  const std::string& p_path = ".",
  const std::string& p_base_name = "log",
  uint32_t p_max_file_size = 10 * 1024 * 1024,
  std::chrono::minutes p_retention = std::chrono::minutes(2 * 24 * 60),
  const std::string& p_closed_extension = "closed");

} // namespace bus
} // namespace logger
} // namespace tenacitas

/// \brief file_set_test sets the log level to 'test'. Aftet this call, all
/// the log messages from 'test' up will de written
#define file_set_test()                                                        \
    tenacitas::logger::bus::cout_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::test)

/// \brief file_set_debug sets the log level to 'debug'. Aftet this call, all
/// the log messages from 'debug' up will de written
#define file_set_debug()                                                       \
    tenacitas::logger::bus::file_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::debug)

/// \brief file_set_info sets the log level to 'info'. Aftet this call, all
/// the log messages from 'info' up will de written
#define file_set_info()                                                        \
    tenacitas::logger::bus::file_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::info)

/// \brief file_set_warn sets the debug level to 'warn'. Aftet this call, all
/// the log messages from 'warn' up will de written
#define file_set_warn()                                                        \
    tenacitas::logger::bus::file_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::warn)

/// \brief file_set_no_log makes no message to be logged to file
#define file_set_no_log()                                                      \
    tenacitas::logger::bus::file_log_t::instance().set_level(                  \
      tenacitas::logger::bus::level::no_log)

/// \brief file_test allows to log with 'test' level
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
#define file_test(p_params...)                                                 \
    tenacitas::logger::bus::file_log_t::instance().test(                       \
      tenacitas::calendar::bus::epoch::millisecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

/// \brief file_debug allows to log with 'debug' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// D|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example file_debug("hello! ", 390) will generate
///
/// D|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define file_debug(p_params...)                                                \
    tenacitas::logger::bus::file_log_t::instance().debug(                      \
      tenacitas::calendar::bus::epoch::microsecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

/// \brief file_info allows to log with 'info' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// I|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example file_info("hello! ", 390) will generate
///
/// I|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define file_info(p_params...)                                                 \
    tenacitas::logger::bus::file_log_t::instance().info(                       \
      tenacitas::calendar::bus::epoch::microsecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

/// \brief file_warn allows to log with 'warning' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// W|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example file_warn("hello! ", 390) will generate
///
/// W|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define file_warn(p_params...)                                                 \
    tenacitas::logger::bus::file_log_t::instance().warn(                       \
      tenacitas::calendar::bus::epoch::microsecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

/// \brief file_error allows to log with 'warning' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// E|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example file_error("hello! ", 390) will generate
///
/// E|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define file_error(p_params...)                                                \
    tenacitas::logger::bus::file_log_t::instance().error(                      \
      tenacitas::calendar::bus::epoch::microsecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

/// \brief file_fatal allows to log with 'warning' level
///
/// \param p_params is a variadic macro parameter (GNU GCC only extension), and
/// each parameter \p p, of type \p t, must implement the \tt std::ostream &
/// operator<<(const t &)\tt operator
///
/// \details the log line will be:
/// F|epoch-microsecs|thread-id|file-name|line-number|other-params
///
/// \example file_fatal("hello! ", 390) will generate
///
/// F|1545843634258841|140072767514368|main.cpp|28|hello! 390
///
#define file_fatal(p_params...)                                                \
    tenacitas::logger::bus::file_log_t::instance().fatal(                      \
      tenacitas::calendar::bus::epoch::microsecs(),                            \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      std::this_thread::get_id(),                                              \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __FILE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      __LINE__,                                                                \
      tenacitas::logger::bus::file_log_t::instance().get_separator(),          \
      p_params)

#endif // FILE_H
