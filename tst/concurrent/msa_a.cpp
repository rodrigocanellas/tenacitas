

#include <concurrent/_tst/msg_a.h>

namespace tenacitas {
namespace concurrent {
namespace _tst {

const char *msg_a::m_s = "012345678901234567890123456789";

msg_a::msg_a(uint32_t p_value) : m_counter(p_value) {}

std::ostream &operator<<(std::ostream &p_out, const msg_a &p_msg) {
  p_out << "(" << formater::_bus::format_number(p_msg.m_counter) << ","
        << msg_a::m_s << "," << p_msg.m_timestamp << ")";
  return p_out;
}

} // namespace _tst
} // namespace concurrent
} // namespace tenacitas