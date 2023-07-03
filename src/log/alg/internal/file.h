#ifndef TENACITAS_LIB_LOG_ALG_FILE_H
#define TENACITAS_LIB_LOG_ALG_FILE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

/// \brief master namespace
namespace tenacitas::lib::log::alg {

/// \brief internal types, objects and functions
namespace internal {

// static constexpr uint32_t max_file_size{5 * 1024 * 1024};

/// \brief Writes log messages to a file
///
/// A maximum file size is defined, and when this size is exceeded, a new file
/// is created
struct file {
  /// \brief Constructor
  ///
  /// \param p_file_name base file name, the actual file name is this name
  /// appended with a timestamp with microsecs precision
  ///
  /// In this case, the maximum file size is 5MB
  file(const std::string &p_file_name) : m_base_name(p_file_name) {
    open_file();
  }

  /// \brief Constructor
  ///
  /// \param p_file_name base file name, the actual file name is this name
  /// appended with a timestamp with microsecs precision
  ///
  /// \param p_size_in_bytes is the maximum file size
  file(const std::string &p_file_name, uint32_t p_size_in_bytes)
      : m_base_name(p_file_name), m_max_size(p_size_in_bytes) {
    open_file();
  }

  /// \brief Writes a string to the file
  void operator()(std::string &&p_str) {
    auto _size = static_cast<decltype(m_max_size)>(p_str.size());
    if ((m_current_size + _size) > m_max_size) {
      open_file();
      m_current_size = 0;
    }
    m_current_size += _size;
    m_stream << p_str;
    m_stream.flush();
  }

private:
  void open_file() {
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
} // namespace internal
} // namespace tenacitas::lib::log::alg
#endif
