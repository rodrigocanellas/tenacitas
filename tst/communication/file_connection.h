#ifndef TENACITAS_COMMUNICATION_TST_FILE_CONNECTION_H
#define TENACITAS_COMMUNICATION_TST_FILE_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <memory>
#include <optional>
#include <thread>

#include <communication/status.h>
#include <communication/internal/log.h>
#include <logger/cerr/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {
/// \brief namespace for test
namespace tst {

struct file_connection_takes_2_secs {
  typedef tenacitas::logger::cerr::log logger;
  file_connection_takes_2_secs() = default;


  status open (const std::string & p_file_name) {
    m_file.open(p_file_name, std::ios::in | std::ios::out);

    if(!m_file.is_open())
    {
      m_file.clear();
      m_file.open(p_file_name, std::ios::out); //Create file.
      m_file.close();
      m_file.open(p_file_name);
    }

    if ( (m_file.bad() || (!m_file.is_open())) ) {
      return status::error_connecting;
    }
    return status::ok;
  }

  template<typename t_char_iterator>
  status send(t_char_iterator p_begin,
              t_char_iterator p_end) {
    comm_log_debug(logger, "going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    comm_log_debug(logger, "waking up");
    m_file.write(&(*p_begin), std::distance(p_begin, p_end));
    m_file.flush();
    if (m_file.bad()) {
      return status::error_sending;
    }
    return status::ok;
  }

  template<typename t_char_iterator>
  std::pair<status,t_char_iterator> receive(t_char_iterator p_begin,
                 t_char_iterator p_end) {
    auto _size = std::distance(p_begin, p_end);
    decltype (_size) _read = static_cast<decltype (_size)>(m_file.readsome(&(*p_begin), _size));

    if (m_file.bad()) {
      return {status::error_sending, p_begin};
    }

    if ( (_read == 0) || (m_file.eof())) {
      return {status::end_of_message, p_begin};
    }

    return {status::ok, std::next(p_begin, _read)};
  }

private:
  std::fstream m_file;
};


} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // FILE_CONNECTION_H
