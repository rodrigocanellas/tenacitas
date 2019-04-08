#include <string/business/split_str.h>

namespace tenacitas {
namespace string {
namespace business {

void
slipt_str(const std::string& p_values,
          std::function<void(std::string&&)> p_handler,
          char p_separator)
{
    std::string::size_type _p1 = 0;
    std::string::size_type _p2 = 0;

    std::string::size_type _size = p_values.size();

    while (true) {
        if (_p1 == _size) {
            break;
        }
        _p2 = p_values.find(p_separator, _p1);
        if (_p2 == std::string::npos) {
            std::string _tmp(p_values.substr(_p1, p_values.size() - _p1));
            p_handler(std::move(_tmp));
            break;
        }

        if (_p2 != _p1) {
            std::string _tmp(p_values.substr(_p1, (_p2 - _p1)));
            p_handler(std::move(_tmp));
        }

        ++_p2;
        _p1 = _p2;
    }
}

} // namespace business
} // namespace string
} // namespace tenacitas
