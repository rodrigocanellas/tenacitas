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

std::ostream&
operator<<(std::ostream& p_out, const types& p_types)
{
    p_out << "{ ";
    static const char* _space = " ";
    for (const type& _type : p_types.m_set) {
        p_out << _type << _space;
    }
    p_out << " }";
    return p_out;
}

types::const_iterator
types::recognize(const std::string& p_str) const
{
    const_iterator _end = m_set.end();
    for (const_iterator _ite = m_set.begin(); _ite != _end; ++_ite) {
        if (_ite->recognize(p_str)) {
            return _ite;
        }
    }
    return _end;
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
