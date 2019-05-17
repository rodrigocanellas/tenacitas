#include <algorithm>

#include <interpreter/business/internal/tokens.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

// -----------------------------------------------------------------------------
std::ostream&
operator<<(std::ostream& p_out, const tokens& p_tokens)
{
    tokens::token_set::const_iterator _token_ite = p_tokens.m_token_set.begin();
    tokens::token_set::const_iterator _token_end = p_tokens.m_token_set.end();
    p_out << "\n";
    while (_token_ite != _token_end) {
        p_out << "size = " << static_cast<uint16_t>(_token_ite->first)
              << std::endl;

        tokens::type_set::const_iterator _type_ite = _token_ite->second.begin();
        tokens::type_set::const_iterator _type_end = _token_ite->second.end();

        while (_type_ite != _type_end) {
            p_out << "\ttype = " << _type_ite->first << std::endl;
            tokens::string_set::const_iterator _string_ite =
              _type_ite->second.begin();
            tokens::string_set::const_iterator _string_end =
              _type_ite->second.end();

            while (_string_ite != _string_end) {
                p_out << "\t\t" << *_string_ite << std::endl;
                ++_string_ite;
            }
            ++_type_ite;
        }
        ++_token_ite;
    }

    return p_out;
}

// -----------------------------------------------------------------------------
type
tokens::recognize(const std::string& p_str) const
{
    type _type(type::undefined);

    if (m_token_set.empty()) {
        // there are no tokens
        return _type;
    }

    uint8_t _size = static_cast<uint8_t>(p_str.size());

    token_set::const_iterator _token_ite = m_token_set.find(_size);
    if (_token_ite == m_token_set.end()) {
        // there is no token of \p _size size
        return _type;
    }

    type_set::const_iterator _type_ite = _token_ite->second.begin();
    type_set::const_iterator _type_end = _token_ite->second.end();

    while (_type_ite != _type_end) {
        if (_type_ite->second.find(p_str) != _type_ite->second.end()) {
            // \p p_str is recognized among the values of the current type
            _type = _type_ite->first;
            break;
        }
        ++_type_ite;
    }
    return _type;
}

// -----------------------------------------------------------------------------
void
tokens::add(const type& p_type, const std::string& p_tokens)
{
    string::business::slipt_str(
      p_tokens, [this, p_type](const std::string& p_str) -> void {
          const uint8_t _size(static_cast<uint8_t>(p_str.size()));
          this->m_token_set[_size][p_type].emplace(p_str);
      });
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
