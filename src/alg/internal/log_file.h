/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ALG_INTERNAL_LOG_FILE_H
#define TENACITAS_LIB_ALG_INTERNAL_LOG_FILE_H

#include <chrono>
#include <fstream>
#include <string>

using namespace std::chrono_literals;

namespace tenacitas::lib::alg::internal {
/// \brief Writes log messages to a log_file
///
/// A maximum log_file size is defined, and when this size is exceeded, a new
/// log_file is created
struct log_file {
  /// \brief Constructor
  ///
    /// \param p_log_file_name base log_file name, the actual log_file name is
  /// this name appended with a timestamp with microsecs precision
  ///
  /// In this case, the maximum log_file size is 5MB
  log_file(const std::string &p_log_file_name)
      : m_base_name(p_log_file_name) {
    open_log_file();
  }

  /// \brief Constructor
  ///
  /// \param p_log_file_name base log_file name, the actual log_file name is
  /// this name appended with a timestamp with microsecs precision
  ///
  /// \param p_size_in_bytes is the maximum log_file size
  log_file(const std::string &p_log_file_name, uint32_t p_size_in_bytes)
      : m_base_name(p_log_file_name), m_max_size(p_size_in_bytes) {
    open_log_file();
  }

  /// \brief Writes a string to the log_file
  void operator()(std::string &&p_str) {
    auto _size = static_cast<decltype(m_max_size)>(p_str.size());
    if ((m_current_size + _size) > m_max_size) {
      open_log_file();
      m_current_size = 0;
    }
    m_current_size += _size;
    m_stream << p_str;
    m_stream.flush();
  }

private:
  void open_log_file() {
    if (m_stream.is_open()) {
      m_stream.close();
    }

    m_current_name = m_base_name + "_" +
                     std::to_string(std::chrono::high_resolution_clock::now()
                                        .time_since_epoch()
                                        .count()) +
                     ".log";
    m_stream.open(m_current_name, std::ios::out);
    if (!m_stream.is_open()) {
      throw std::runtime_error("could not open '" + m_current_name + "'");
    }
  }

private:
  std::string m_base_name;
  std::string m_current_name;
  std::ofstream m_stream;
  uint32_t m_max_size{5 * 1024 * 1024};
  uint32_t m_current_size{0};
};

} // namespace tenacitas::lib::alg::internal

namespace tla = tenacitas::lib::alg;
#endif
