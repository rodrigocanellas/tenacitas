#ifndef TENACITAS_CONCURRENT_CONCURRENT_LOG_H
#define TENACITAS_CONCURRENT_CONCURRENT_LOG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <string>

#define concurrent_debug(log, p_params...) log.debug(__LINE__, p_params)

#define concurrent_info(log, p_params...) log.info(__LINE__, p_params)

#define concurent_warn(log, p_params...) log.warn(__LINE__, p_params)

#define concurrent_error(log, p_params...) log.error(__LINE__, p_params)

#define concurrent_fatal(log, p_params...) log.fatal(__LINE__, p_params)

#endif // concurrent_log_debug_H
