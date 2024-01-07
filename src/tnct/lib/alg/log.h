/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_ALG_INTERNAL_LOG_H
#define TNCT_LIB_ALG_INTERNAL_LOG_H

#include <tnct/lib/alg/internal/log_file.h>
#include <tnct/lib/alg/internal/log_wrapper.h>
#include <tnct/lib/dat/internal/max_log_file_name_lenght.h>

namespace tnct::lib::alg {

///// \brief Sets the global log level as 'trace'
inline void set_trace_level() { internal::log_wrapper<>::set_trace(); }

/// \brief Sets the global log level as 'debug'
inline void set_debug_level() { internal::log_wrapper<>::set_debug(); }

/// \brief Sets the global log level as 'info'
inline void set_info_level() { internal::log_wrapper<>::set_info(); }

/// \brief Sets the global log level as 'warn'
inline void set_warn_level() { internal::log_wrapper<>::set_warn(); }

/// \brief Defines that log messages will be written to \p std::cerr
inline void set_writer_cerr() {
  internal::log_wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::cerr << p_str; });
}

/// \brief Defines that log messages will be written to \p std::cout
inline void set_writer_cout() {
  internal::log_wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::cout << p_str; });
}

/// \brief Defines that log messages will be written to \p std::clog
inline void set_writer_clog() {
  internal::log_wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::clog << p_str; });
}

/// \brief Defines the function used to write the log messages
inline void set_writer(std::function<void(std::string &&)> p_writer) {
  internal::log_wrapper<>::set_writer(p_writer);
}

inline void set_log_file_writer(const std::string &p_base_log_file_name,
                                uint32_t p_max_log_file_size = 5 * 1024 *
                                                               1024) {
  auto _log_file{std::make_shared<internal::log_file>(p_base_log_file_name,
                                                      p_max_log_file_size)};

  internal::log_wrapper<>::set_writer([_log_file](std::string &&p_str) -> void {
    (*_log_file)(std::move(p_str));
  });
}

inline void set_max_log_file_name_lenght(uint8_t p_value) {
  tld::internal::max_log_file_name_lenght = p_value;
}

} // namespace tnct::lib::alg

namespace tla = tnct::lib::alg;

/// \brief log_wrapper to the test log function

#define TNCT_LOG_TST(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::test(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the trace log function
#define TNCT_LOG_TRA(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::trace(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the debug log function
#define TNCT_LOG_DEB(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::debug(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the info log function
#define TNCT_LOG_INF(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::info(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the warn log function
#define TNCT_LOG_WAR(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::warn(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the error log function
#define TNCT_LOG_ERR(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::error(__FILE__, __LINE__, p_params)

/// \brief log_wrapper to the fatal log function
#define TNCT_LOG_FAT(p_params...)                                              \
  tnct::lib::alg::internal::log_wrapper<>::fatal(__FILE__, __LINE__, p_params)

/// \brief Defines the character to separate the fields in a log message
#define TNCT_LOG_SEP(separator)                                                \
  tnct::lib::alg::internal::log_wrapper<>::set_separator(separator)

#endif
