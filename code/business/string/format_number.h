#ifndef TENACITAS_STRING_BUS_FORMAT_NUMBER_H
#define TENACITAS_STRING_BUS_FORMAT_NUMBER_H

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <business/string/max_str_length.h>

namespace tenacitas {
namespace business {
namespace string {

template<typename t_num_type>
std::string
format_number(t_num_type p_num) {
    std::stringstream _stream;
    _stream << std::right
            << std::setfill('0') << std::setw(max_str_length<t_num_type>()) << p_num;
    return _stream.str();
}

} // namespace string
} // namespace business
} // namespace tenacitas

#endif // TENACITAS_STRING_FORMAT_NUMBER_H
