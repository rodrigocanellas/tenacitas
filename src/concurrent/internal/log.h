#ifndef TENACITAS_CONCURRENT_CONCURRENT_p_log_H
#define TENACITAS_CONCURRENT_CONCURRENT_p_log_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \brief Wraps to the debug log function
#define concurrent_debug(p_log, p_params...) p_log.debug(__LINE__, p_params)

/// \brief Wraps to the info log function
#define concurrent_info(p_log, p_params...) p_log.info(__LINE__, p_params)

/// \brief Wraps to the warn log function
#define concurrent_warn(p_log, p_params...) p_log.warn(__LINE__, p_params)

/// \brief Wraps to the error log function
#define concurrent_error(p_log, p_params...) p_log.error(__LINE__, p_params)

/// \brief Wraps to the fatal log function
#define concurrent_fatal(p_log, p_params...) p_log.fatal(__LINE__, p_params)

#endif // concurrent_p_log_debug_H
