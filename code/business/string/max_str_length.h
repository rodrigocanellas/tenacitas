#ifndef TENACITAS_STRING_BUS_MAX_STR_LENGTH_H
#define TENACITAS_STRING_BUS_MAX_STR_LENGTH_H

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>


namespace tenacitas {
namespace string {
namespace business {

template<typename t_num_type>
uint16_t
max_str_length()
{
    //    uint16_t _eight(8u);
    //    uint16_t _two(2u);
    uint16_t _log(
      static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))));

    return ++_log;
}

} // namespace business
} // namespace string
} // namespace tenacitas

#endif // MAX_STR_LENGTH_H
