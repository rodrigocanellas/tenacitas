#ifndef TENACITAS_BUSINESS_LOGGER_CERR_H
#define TENACITAS_BUSINESS_LOGGER_CERR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

#include <business/logger/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace business {
/// \brief namespace of the class group
namespace logger {

///
/// \brief configure_cerr_log configures logging to \p cerr
///
/// This namespace function must be called only one time to configure logging to
/// \p cerr
///
/// \details There are 6 logger::log levels, from the lowest: 'test', 'debug',
/// 'info', 'warn', 'error' and 'fatal'. The logger::log level starts in
/// 'error', so only 'error' and 'fatal' messages will be logged.
///
/// If one wants to change the logger::log level, the 'cerr_set_test',
/// 'cerr_set_debug', 'cerr_set_info' or 'cerr_set_warn' macro command must be
/// called
///
/// A logger::log line has this format:
/// logger::log-level|timestamp-millisecs|thread-id|file|line|user-contents
/// For example:
/// T|1552098707355|140299908863808|thread_pool_tester.h|59|adding
/// (0000000005,012345678901234567890123456789,1552098707355)
///
inline void
configure_cerr_log()
{
  logger::log::configure([](std::string&& p_str) { std::cerr << p_str; });
}

} // namespace logger
} // namespace business
} // namespace tenacitas

#endif
