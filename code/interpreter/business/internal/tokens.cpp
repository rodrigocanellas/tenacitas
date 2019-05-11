#include <interpreter/business/internal/tokens.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

type
tokens::recognize(const std::string& p_str) const
{

    if (m_set.empty()) {
        return type::unreconized;
    }

    uint8_t _size = static_cast<uint8_t>(p_str.size());

    set_t::const_iterator _ite = m_set.begin();
    set_t::const_iterator _end = m_set.end();

    while (true) {
        if ((_ite == _end) || (_ite->get_size() > _size)) {
            // there is no token with \p _size size
            break;
        }
        if (_ite->get_size() == _size) {
            if (_ite->recognize(p_str)) {
                return _ite->get_type();
            }
            break;
        }
        ++_ite;
    }
    return type::unreconized;
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
