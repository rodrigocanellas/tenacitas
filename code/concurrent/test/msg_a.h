#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <cstdint>
#include <iostream>
#include <sstream>

#include <calendar/business/epoch.h>
#include <string/business/format_number.h>

namespace tenacitas {
namespace concurrent {
namespace  test  {


struct msg_a
{
    explicit msg_a(uint32_t p_value = 0);
    friend std::ostream& operator<<(std::ostream& p_out, const msg_a&);
    inline uint32_t counter() const { return m_counter; }

  private:
    static const char* m_s;
    uint32_t m_counter = 0;
    uint64_t m_timestamp = calendar::business::epoch::millisecs();
};



} // namespace test
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
