#ifndef TENACITAS_LOGGER_BUSINESS_FILE_H
#define TENACITAS_LOGGER_BUSINESS_FILE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include <logger/business/internal/file_controller.h>
#include <logger/business/internal/file_writer.h>
#include <logger/business/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the class group
namespace business {

///
/// \brief configure_file_log configures logging in file
///
/// This namespace function must be called only one time to configure logging to
/// a file
///
/// \param p_path path is where the logger::log files will be generated
/// \param p_base_name first string in the logger::log file name; one can use, for
/// example, the name of the program
/// \param p_max_file_size maximum size of the file, in bytes
/// \param p_retention amount of time that the file will exist, before being
/// deleted, in minutes
/// \param p_closed_extension is a string that will be appended to the file
/// name, showing that it was closed, because it has reached its maximum size,
/// or the program has finished.
///
/// \details Using the default values, logger::log files will be created at the current
/// path; the name of the files will begin with 'logger::log'; when it reaches 10MB a
/// new logger::log file will be created; each logger::log file will last up to 2 days
///
/// A logger::log file name is the combination of 'p_base_name' concatenated
/// with the process id (PID), and with the amount of milliseconds of when the
/// file was created, like: 'log_0000008165_1545843474201246.logger::log'.
///
/// When this files reaches (almost) 10MB, it will be renamed to
/// 'log_0000008165_1545843474201246.logger::log.closed'.
///
/// When it is more that 2 days old, it will be deleted.
///
/// \details There are 6 logger::log levels, from the lowest: 'test', 'debug', 'info',
/// 'warn', 'error' and 'fatal'. The logger::log level starts in 'error', so only
/// 'error' and 'fatal' messages will be logged.
///
/// If one wants to change the logger::log level, the 'file_set_test', 'file_set_debug',
/// 'file_set_info' or 'file_set_warn' must be called
///
/// A logger::log line has this format:
/// logger::log-level|timestamp-millisecs|thread-id|file|line|user-contents
///
/// For example
/// T|1552098707355|140299908863808|thread_pool_tester.h|59|adding
/// (0000000005,012345678901234567890123456789,1552098707355)
///
inline void
configure_file_log(
  const std::string& p_path = ".",
  const std::string& p_base_name = "logger::log",
  uint32_t p_max_file_size = 10 * 1024 * 1024,
  std::chrono::minutes p_retention = std::chrono::minutes(2 * 24 * 60),
  const std::string& p_closed_extension = "closed")
{

    file_controller _file_controller(
      p_path, p_base_name, p_max_file_size, p_retention, p_closed_extension);

    typedef file_writer_t<file_controller> file_writer;
    typedef std::shared_ptr<file_writer> file_writer_ptr;

    file_writer_ptr _file_writer_ptr(
      std::make_shared<file_writer>(std::move(_file_controller)));

    //    logger::log::configure(std::move(_file_writer));
    log::configure([_file_writer_ptr](std::string&& p_str) -> void {
        (*_file_writer_ptr)(std::move(p_str));
    });
}

} // namespace logger
} // namespace business
} // namespace tenacitas

#endif // FILE_H
