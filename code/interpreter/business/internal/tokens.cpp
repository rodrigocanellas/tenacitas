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

bool
tokens::add(type::id&& p_id, std::string&& p_tokens)
{
    const std::string::size_type _pos(p_tokens.find(' '));
    if (_pos == std::string::npos) {
        return false;
    }

    const std::string _str(p_tokens.substr(0, _pos));

    const uint8_t _size(static_cast<uint8_t>(_str.size()));

    string::business::slipt_str(
      p_tokens, [_size](const std::string& p_str) -> void {
          if (static_cast<uint8_t>(p_str.size()) != _size) {
              throw std::out_of_range("Size of " + p_str + " is not " +
                                      std::to_string(_size));
          }
      });

    token_recognizer _recognize(std::move(p_tokens));

    type _type(std::move(p_id), std::move(_recognize));

    return m_set.emplace(token(_size, std::move(_type))).second;
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
