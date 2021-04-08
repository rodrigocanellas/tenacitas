#ifndef TENACITAS_CONCURRENT_TST_FILE_STREAM_H
#define TENACITAS_CONCURRENT_TST_FILE_STREAM_H

#include <fstream>
#include <string>

#include <concurrent/result.h>
#include <status/result.h>

namespace tenacitas {
namespace concurrent {
namespace tst {
struct file {
  file(const std::string &p_name) : m_stream(p_name) {}

  bool operator()(const char *p_data, size_t p_size) {
    m_stream.write(p_data, p_size);
    return m_stream.good();
  }

private:
  std::ofstream m_stream;
};

} // namespace tst
} // namespace concurrent
} // namespace tenacitas

#endif // FILE_STREAM_H
