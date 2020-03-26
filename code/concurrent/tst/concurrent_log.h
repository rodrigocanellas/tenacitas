#ifndef TENACITAS_LOGGER_TST_CONCURRENT_LOG_H
#define TENACITAS_LOGGER_TST_CONCURRENT_LOG_H

#include <logger/cerr/log.h>

#define concurrent_log(...)                                                    \
  tenacitas::logger::cerr::log::debug("concurrent", __LINE__, __VA_ARGS__)

#endif // CONCURRENT_LOG_H
