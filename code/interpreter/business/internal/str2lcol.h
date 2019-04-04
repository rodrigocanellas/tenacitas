#ifndef STR2COL_H
#define STR2COL_H

#include <string>

namespace tenacitas {
namespace interpreter {
namespace business {

template<typename t_collection>
void
str2col(const std::string& p_values, t_collection& p_collection)
{
    typedef typename t_collection::value_t value_t;
    std::string::size_type _p1 = 0;
    std::string::size_type _p2 = _p1;

    while (true) {
        _p2 = p_values.find(' ', _p1);
        if (_p2 == std::string::npos) {
            break;
        }
        p_collection.add(value_t(p_values.substr(_p1, _p2)));
        ++_p2;
        _p1 = _p2;
    }
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif // STR2LST_H
