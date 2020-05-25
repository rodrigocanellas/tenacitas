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
  explicit inline file_connection_takes_2_secs(std::fstream &&p_file)
      : m_file(std::move(p_file)) {}

  status send(std::string::const_iterator p_begin,
              std::string::const_iterator p_end) {
    comm_log_debug(logger, "going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    comm_log_debug(logger, "waking up");
    m_file.write(&(*p_begin), std::distance(p_begin, p_end));
    if (m_file.bad()) {
      return status::error_sending;
    }
    return status::ok;
  }

  std::pair<status, std::string::const_iterator> receive(std::string::iterator p_begin, std::string::iterator p_end) {
    auto _buffer_size = std::distance(p_begin, p_end);
    typedef decltype (_buffer_size)  size;
    size _amount_read = static_cast<size>(m_file.readsome(&(*p_begin), _buffer_size));
    if (_amount_read == 0) {
      return {status::end_of_message, p_begin};
    }
    if (_amount_read == _buffer_size) {
      return {status::ok, p_end};
    }
    std::string::const_iterator _end = std::next(p_begin,_amount_read);
    return {status::ok, _end};

  }

  inline uint16_t get_send_size() const {return 50;}
  inline uint16_t get_receive_size() const {return 10;}

private:
  std::fstream m_file;
};

struct file_connector {

  typedef std::shared_ptr<file_connector> ptr;
  typedef file_connection_takes_2_secs connection;

  static ptr create() {
    return std::make_shared<file_connector>();
  }

  inline std::pair<status, connection> connect(const std::string &p_file_name) {
    std::fstream _file(p_file_name, std::ios::trunc);
    if (_file.bad()) {
      return {status::error_connecting,connection()};
    }
    connection _connection(std::move(_file));
    return {status::ok, std::move(_connection)};
  }
};

} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // FILE_CONNECTION_H
