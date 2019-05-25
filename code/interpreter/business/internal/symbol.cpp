#include <interpreter/business/internal/symbol.h>

namespace tenacitas {
namespace interpreter {
namespace business {

const symbol symbol::eot(lexeme("EOT"), type(".EOT."));

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

} // namespace business
} // namespace interpreter
} // namespace tenacitas
