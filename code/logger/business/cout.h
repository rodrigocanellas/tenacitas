#ifndef TENACITAS_LOGGER_BUS_COUT_H
#define TENACITAS_LOGGER_BUS_COUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <iostream>
#include <thread>

#include <logger/business/log.h>

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
inline void
configure_cout_log()
{
    log::configure([](std::string&& p_str) { std::cout << p_str; });
}

} // namespace business
} // namespace logger
} // namespace tenacitas

#endif
