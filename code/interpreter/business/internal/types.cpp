#include <interpreter/business/internal/types.h>

namespace tenacitas {
namespace interpreter {
namespace business {

types::const_iterator
types::add(type&& p_type)
{
    std::pair<std::set<type>::iterator, bool> _ret = m_set.insert(p_type);
    if (_ret.second) {
        return _ret.first;
    }
    if ((_ret.first != m_set.end()) && (*_ret.first == p_type)) {
        return _ret.first;
    }
    return this->end();
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
