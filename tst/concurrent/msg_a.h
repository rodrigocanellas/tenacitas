#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>

//#include <calendar/epoch.h>
#include <number/format_000.h>

namespace tenacitas {
namespace concurrent {

template <typename t_int = int16_t> struct msg_a {
  typedef t_int data;
  explicit msg_a(t_int p_value = std::numeric_limits<t_int>::max())
      : m_counter(p_value) {}
  friend std::ostream &operator<<(std::ostream &p_out, const msg_a &p_msg) {
    //    p_out << "(" << number::format_000(p_msg.m_counter) << "," <<
    //    msg_a::m_s
    //          << "," << p_msg.m_timestamp << ")";
    p_out << "(" << number::format_000(p_msg.m_counter) << ")";
    return p_out;
  }
  inline t_int counter() const { return m_counter; }

private:
  static const char *m_s;
  t_int m_counter = 0;
  uint64_t m_timestamp = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count());
};

template <typename t_int>
const char *msg_a<t_int>::m_s = "## mensagem a -- mensagem a ##";

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
