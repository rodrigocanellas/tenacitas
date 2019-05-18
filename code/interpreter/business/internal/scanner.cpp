#include "scanner.h"

using namespace tenacitas::interpreter;

/// \brief Namespace of the organization
namespace tenacitas {
/// \brief Identifiers to allow the creation of a interpreter
namespace interpreter {

namespace business {

//----------------------------------------------------------------------------
void
scanner::set_text(std::string::const_iterator p_begin,
                  std::string::const_iterator p_end)
{
    m_curr_line = 1;
    m_curr_col = 1;
    m_begin = p_begin;
    m_end = p_end;
    m_walker = m_begin;
    m_line_last_symbol = 1;
    m_col_last_symbol = 1;
}

//----------------------------------------------------------------------------
symbol
scanner::get_symbol()
{

    // before anything, let's skip the chars defined to be ignored
    skip();

    if (is_eot()) {
        // we reached the end of the text
        return symbol::eot;
    }

    // trying to recognize by value
    type _type = m_tokens.recognize("");

    if (_type == type::undefined) {
        // trying to recognize by type
        _type = m_recognizers.recognize("");
    }

    return symbol(lexeme(""), std::move(_type));
}

//----------------------------------------------------------------------------
void
scanner::skip()
{
    while (true) {

        if (*m_walker == '\n') {
            ++m_curr_line;
            m_curr_col = 1;
            //             ++m_walker;
            //             break;
        }

        if (m_to_ignore.find(*m_walker) == std::string::npos) {
            break;
        }

        ++m_curr_col;

        if (is_eot()) {
            break;
        }

        ++m_walker;
    }
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
