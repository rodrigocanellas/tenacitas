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
namespace test {

template<char id = 'A'>
struct msg
{
  typedef uint64_t number;
  explicit msg(number p_value = 1)
    : m_counter(p_value)
    , m_up(
        tenacitas::number::format(std::numeric_limits<number>::min() + p_value))
    , m_down(tenacitas::number::format(tenacitas::number::format(
        std::numeric_limits<number>::max() - p_value)))

  {
    m_d = 2.5 * p_value;
  }

  friend std::ostream& operator<<(std::ostream& p_out, const msg& p_msg)
  {
    p_out << "(" << id << "," << p_msg.m_counter << "," << p_msg.m_up << ","
          << p_msg.m_down << "," << p_msg.m_d << ")";
    return p_out;
  }
  inline number value() const { return m_counter; }
  inline void inc() { ++m_counter; }
  inline msg& operator++()
  {
    ++m_counter;
    m_up = tenacitas::number::format(m_counter);
    m_down =
      tenacitas::number::format(std::numeric_limits<number>::min() - m_counter);
    m_d = 2.5 * m_counter;
    return *this;
  }

private:
  number m_counter{ std::numeric_limits<number>::min() };
  std::string m_up;
  std::string m_down;
  double m_d;
};

} // namespace test
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
