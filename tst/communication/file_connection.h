#ifndef TENACITAS_COMMUNICATION_TST_FILE_CONNECTION_H
#define TENACITAS_COMMUNICATION_TST_FILE_CONNECTION_H

#include <cstdint>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>

#include <communication/status.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {
/// \brief namespace for test
namespace tst {

struct file_connection {
  file_connection() = default;
  explicit inline file_connection(std::ofstream &&p_file)
      : m_file(std::move(p_file)) {}

  status send(std::string::const_iterator p_begin,
              std::string::const_iterator p_end) {
    m_file.write(&(*p_begin), std::distance(p_begin, p_end));
    if (m_file.bad()) {
      return status::error_sending;
    }
    return status::ok;
  }

  uint16_t get_io_size() const {return 50;}

private:
  std::ofstream m_file;
};

struct file_connector {

  typedef file_connection connection;

  inline std::pair<status, connection> connect(const std::string &p_file_name) {
    std::ofstream _file(p_file_name);
    connection _connection(std::move(_file));
    std::pair<status, connection> _res{status::ok, std::move(_connection)};
    return _res;
  }
};

} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // FILE_CONNECTION_H
