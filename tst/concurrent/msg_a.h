#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>

//#include <calendar/epoch.h>

#include <tenacitas/number.h>

namespace tenacitas {
namespace concurrent {

struct msg_a
{
  typedef uint64_t number;
  explicit msg_a(number p_value = std::numeric_limits<number>::min())
    : m_counter(p_value)
    , m_up(tenacitas::number::format(p_value))
    , m_down(
        tenacitas::number::format(std::numeric_limits<number>::min() - p_value))
    , m_d(3.14 * p_value)
  {}

  friend std::ostream& operator<<(std::ostream& p_out, const msg_a& p_msg)
  {
    p_out << "(" << p_msg.m_counter << "," << p_msg.m_up << "," << p_msg.m_down
          << "," << p_msg.m_d << ")";
    return p_out;
  }
  inline number value() const { return m_counter; }
  inline void inc() { ++m_counter; }
  inline msg_a& operator++()
  {
    ++m_counter;
    m_up = tenacitas::number::format(m_counter);
    m_down =
      tenacitas::number::format(std::numeric_limits<number>::min() - m_counter);
    m_d = 3.14 * m_counter;
    return *this;
  }

private:
  number m_counter{ std::numeric_limits<number>::min() };
  std::string m_up;
  std::string m_down;
  double m_d;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
