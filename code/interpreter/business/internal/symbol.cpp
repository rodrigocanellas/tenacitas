#include <interpreter/business/internal/symbol.h>

namespace tenacitas {
namespace interpreter {
namespace business {

std::ostream&
operator<<(std::ostream& p_out, const symbol& p_symbol)
{
    static const char* _space = " ";
    static const char* _open = "[";
    static const char* _close = "]";
    p_out << _open << p_symbol.m_lexeme << _space << p_symbol.m_type << _close
          << _space;
    return p_out;
}

// const lexeme symbol::eot("!e!");

// const lexeme symbol::m_dummy_lexeme("");

// const type symbol::m_dummy_type("");

} // namespace business
} // namespace interpreter
} // namespace tenacitas
