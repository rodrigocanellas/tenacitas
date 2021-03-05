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
      : m_counter(p_value),
    m_str(tenacitas::number::format(p_value)){}

  friend std::ostream &operator<<(std::ostream &p_out, const msg_a &p_msg) {
    p_out << "(" << p_msg.m_counter << "," << p_msg.m_str << ")";
    return p_out;
  }
  inline int32_t value() const { return m_counter; }
  inline void inc() { ++m_counter; }

private:
  int32_t m_counter {std::numeric_limits<int32_t>::min()};
  std::string m_str;

};


} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
