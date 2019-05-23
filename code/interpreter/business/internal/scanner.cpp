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
    m_current = m_begin;
    m_walker = m_current;
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

    //    symbol _symbol = recognize_by_token();
    //    if (_symbol.get_type() == type::undefined) {
    //        _symbol = recognize_by_type();
    //    }

    symbol _symbol = recognize([this](const std::string& p_str) -> type {
        return this->m_tokens.recognize(p_str);
    });

    if (_symbol.get_type() == type::undefined) {
        _symbol = recognize([this](const std::string& p_str) -> type {
            return this->m_recognizers.recognize(p_str);
        });
    }

    m_curr_col = static_cast<column>(std::distance(m_current, m_walker));
    m_current = m_walker;
    m_line_last_symbol = m_curr_line;
    m_col_last_symbol = m_curr_col;

    return _symbol;
}

//----------------------------------------------------------------------------
void
scanner::skip()
{
    while (true) {

        if (*m_current == '\n') {
            ++m_curr_line;
            m_curr_col = 1;
            //             ++m_current;
            //             break;
        }

        if (m_to_ignore.find(*m_current) == std::string::npos) {
            break;
        }

        ++m_curr_col;

        if (is_eot()) {
            break;
        }

        ++m_current;
    }
}

//----------------------------------------------------------------------------
symbol
scanner::recognize(recognizer p_recognizer)
{
    m_walker = m_current;
    while (true) {

        // any of these conditions makes us stop
        if ((m_walker == m_end) || (*m_walker == '\0') || (*m_walker == ' ') ||
            (*m_walker == '\n')) {
            break;
        }

        ++m_walker;
    }

    type _type(type::undefined);
    std::string _str;
    if (std::distance(m_current, m_walker) == 0) {
        _str = std::string(std::string(1, *m_walker));
        _type = p_recognizer(_str);
    } else {
        _str = std::string(std::string(m_current, m_walker));
        _type = p_recognizer(_str);
    }
    return symbol(lexeme(_str), _type);
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
