#ifndef TENACITAS_CONCURRENT_CONCURRENT_LOG_H
#define TENACITAS_CONCURRENT_CONCURRENT_LOG_H

#include <string/business/file_name_from_path.h>

#define concurrent_log_test(log, p_params...)                                  \
  log::test(tenacitas::string::business::file_name_from_path(__FILE__),        \
            __LINE__,                                                          \
            p_params)

#define concurrent_log_debug(log, p_params...)                                 \
  log::debug(tenacitas::string::business::file_name_from_path(__FILE__),       \
             __LINE__,                                                         \
             p_params)

#define concurrent_log_info(log, p_params...)                                  \
  log::info(tenacitas::string::business::file_name_from_path(__FILE__),        \
            __LINE__,                                                          \
            p_params)

#define concurrent_log_warn(log, p_params...)                                  \
  log::warn(tenacitas::string::business::file_name_from_path(__FILE__),        \
            __LINE__,                                                          \
            p_params)

#define concurrent_log_error(log, p_params...)                                 \
  log::error(tenacitas::string::business::file_name_from_path(__FILE__),       \
             __LINE__,                                                         \
             p_params)

#define concurrent_log_fatal(log, p_params...)                                 \
  log::fatal(tenacitas::string::business::file_name_from_path(__FILE__),       \
             __LINE__,                                                         \
             p_params)

#endif // concurrent_log_H
