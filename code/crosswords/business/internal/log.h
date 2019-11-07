#ifndef TENACITAS_CROSSWORDS_BUSINESS_LOG_H
#define TENACITAS_CROSSWORDS_BUSINESS_LOG_H

#include <algorithm>
#include <cstring>
#include <string>

const char*
file_name(const char* p_path)
{
  const char* _p = p_path + strlen(p_path);
  while (true) {
    if (_p == p_path) {
      return _p;
    }
    if ((*_p == '/') || (*_p == '\\')) {
      return ++_p;
    }
    --_p;
  }
}

#define crosswords_log_test(log, p_params...)                                  \
  log::test(file_name(__FILE__), __LINE__, p_params)

#define crosswords_log_debug(log, p_params...)                                 \
  log::debug(file_name(__FILE__), __LINE__, p_params)

#define crosswords_log_info(log, p_params...)                                  \
  log::info(file_name(__FILE__), __LINE__, p_params)

#define crosswords_log_warn(log, p_params...)                                  \
  log::warn(file_name(__FILE__), __LINE__, p_params)

#define crosswords_log_error(log, p_params...)                                 \
  log::error(file_name(__FILE__), __LINE__, p_params)

#define crosswords_log_fatal(log, p_params...)                                 \
  log::fatal(file_name(__FILE__), __LINE__, p_params)

#endif // LOG_H
