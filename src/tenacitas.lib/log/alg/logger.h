#ifndef TENACITAS_LIB_LOG_ALG_LOGGER_H
#define TENACITAS_LIB_LOG_ALG_LOGGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <thread>

#ifdef TENACITAS_LOG

#include <tenacitas.lib/log/alg/internal/file.h>
#include <tenacitas.lib/log/alg/internal/wrapper.h>
#include <tenacitas.lib/log/typ/level.h>
#include <tenacitas.lib/number/alg/format.h>
#include <tenacitas.lib/traits/alg/traits.h>

/// \brief Wrapper to the test log function
#define TNCT_LOG_TST(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::test(__FILE__, __LINE__, \
                                                           p_params)

/// \brief Wrapper to the trace log function
#define TNCT_LOG_TRA(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::trace(                   \
      __FILE__, __LINE__, p_params)

/// \brief Wrapper to the debug log function
#define TNCT_LOG_DEB(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::debug(                   \
      __FILE__, __LINE__, p_params)

/// \brief Wrapper to the info log function
#define TNCT_LOG_INF(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::info(__FILE__, __LINE__, \
                                                           p_params)

/// \brief Wrapper to the warn log function
#define TNCT_LOG_WAR(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::warn(__FILE__, __LINE__, \
                                                           p_params)

/// \brief Wrapper to the error log function
#define TNCT_LOG_ERR(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::error(                   \
      __FILE__, __LINE__, p_params)

/// \brief Wrapper to the fatal log function
#define TNCT_LOG_FAT(p_params...)                                              \
  tenacitas::lib::log::alg::internal::wrapper<>::fatal(                   \
      __FILE__, __LINE__, p_params)

/// \brief Defines the character to separate the fields in a log message
#define TNCT_LOG_SEP(separator)                                                \
  tenacitas::lib::log::alg::internal::wrapper<>::set_separator(separator)

/// \brief master namespace
namespace tenacitas::lib::log::alg {

///// \brief Sets the global log level as 'trace'
inline void set_trace_level() { internal::wrapper<>::set_trace(); }

/// \brief Sets the global log level as 'debug'
inline void set_debug_level() { internal::wrapper<>::set_debug(); }

/// \brief Sets the global log level as 'info'
inline void set_info_level() { internal::wrapper<>::set_info(); }

/// \brief Sets the global log level as 'warn'
inline void set_warn_level() { internal::wrapper<>::set_warn(); }

/// \brief Defines that log messages will be written to \p std::cerr
inline void set_writer_cerr() {
  internal::wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::cerr << p_str; });
}

/// \brief Defines that log messages will be written to \p std::cout
inline void set_writer_cout() {
  internal::wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::cout << p_str; });
}

/// \brief Defines that log messages will be written to \p std::clog
inline void set_writer_clog() {
  internal::wrapper<>::set_writer(
      [](std::string &&p_str) -> void { std::clog << p_str; });
}

/// \brief Defines the function used to write the log messages
inline void set_writer(std::function<void(std::string &&)> p_writer) {
  internal::wrapper<>::set_writer(p_writer);
}

inline void set_file_writer(const std::string &p_base_file_name,
                            uint32_t p_max_file_size = 5 * 1024 * 1024) {
  auto _file{
      std::make_shared<internal::file>(p_base_file_name, p_max_file_size)};

  internal::wrapper<>::set_writer(
      [_file](std::string &&p_str) -> void { (*_file)(std::move(p_str)); });
}

inline void set_max_file_name_lenght(uint8_t p_value) {
  internal::max_file_name_lenght = p_value;
}

} // namespace tenacitas::lib::log::alg

#else

// dummy log implementation
/// \brief Wrapper to the test log function
#define TNCT_LOG_TST(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the debug log function
#define TNCT_LOG_TRA(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the debug log function
#define TNCT_LOG_DEB(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the info log function
#define TNCT_LOG_INF(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the warn log function
#define TNCT_LOG_WAR(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the error log function
#define TNCT_LOG_ERR(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Wraps to the fatal log function
#define TNCT_LOG_FAT(p_params...)                                              \
  { tenacitas::lib::log::alg::dummy(std::cout, p_params); }

/// \brief Defines the character to separate the fields in a log
/// message
#define LOG_SEP(separator)                                                     \
  {}

/// \brief master namespace
namespace tenacitas::lib::log::alg {

template <typename... t_params>
inline void dummy([[maybe_unused]] std::ostream &p_out,
                  [[maybe_unused]] t_params &&...p_params) {}

inline void set_trace_level() {}
inline void set_debug_level() {}
inline void set_info_level() {}
inline void set_warn_level() {}
inline void set_writer_cerr() {}
inline void set_writer_cout() {}
inline void set_writer_clog() {}
inline void set_max_file_name_lenght(uint8_t) {}

} // namespace tenacitas::lib::log::alg

#endif

#endif // TENACITAS_LOGGER_H
