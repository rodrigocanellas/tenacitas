#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <sstream>

#include <calendar/bus/epoch.h>
#include <formater/bus/format_number.h>

namespace tenacitas {
namespace concurrent {
namespace tst {

struct msg_a {
  explicit msg_a(uint32_t p_value = 0);
  friend std::ostream &operator<<(std::ostream &p_out, const msg_a &);
  inline uint32_t counter() const { return m_counter; }

private:
  static const char *m_s;
  uint32_t m_counter = 0;
  uint64_t m_timestamp = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count());
};

} // namespace tst
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
