#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <cstdint>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>

//#include <calendar/epoch.h>
#include <tenacitas/number.h>

namespace tenacitas {
namespace concurrent {

struct msg_a {
  typedef int32_t number;
  explicit msg_a(number p_value = std::numeric_limits<int32_t>::min())
      : m_counter(p_value) {}
  friend std::ostream &operator<<(std::ostream &p_out, const msg_a &p_msg) {
    p_out << number::format(p_msg.m_counter);
    return p_out;
  }
  inline int32_t value() const { return m_counter; }
  inline void inc() { ++m_counter; }

private:
//  static const char *m_s;
  int32_t m_counter {std::numeric_limits<int32_t>::min()};
//  uint64_t m_timestamp = static_cast<uint64_t>(
//      std::chrono::duration_cast<std::chrono::milliseconds>(
//          std::chrono::high_resolution_clock::now().time_since_epoch())
//          .count());
};

//template <typename t_int>
//const char *msg_a<t_int>::m_s = "## mensagem a -- mensagem a ##";

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
