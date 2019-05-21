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

    symbol _symbol = recognize_by_token();
    if (_symbol.get_type() == type::undefined) {
        _symbol = recognize_by_type();
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
scanner::recognize_by_type()
{

    m_walker = m_current;
    ++m_walker;
    while (true) {

        // any of these conditions makes us stop
        if ((m_walker == m_end) || (*m_walker == '\0') || (*m_walker == ' ') ||
            (*m_walker == '\n')) {
            break;
        }

        ++m_walker;
    }

    const std::string _str(m_current, m_walker);
    type _type = m_recognizers.recognize(_str);
    if (_type != type::undefined) {
        return symbol(lexeme(_str), _type);
    }
    return symbol(lexeme(_str), type::undefined);
}

// ----------------------------------------------------------------------------
symbol
scanner::recognize_by_token()
{

    type _type(type::undefined);
    m_walker = m_current;
    bool _ever_recognized = false;

    if (!m_tokens.empty()) {

        while (true) {

            // any of these conditions makes us stop
            if ((m_walker == m_end) || (*m_walker == '\0') ||
                (*m_walker == ' ') || (*m_walker == '\n')) {
                break;
            }

            // if the string value parsed so far matches one of the registered
            // tokens
            if (std::distance(m_current, m_walker) == 0) {
                const std::string _str(std::string(1, *m_walker));
                _type = m_tokens.recognize(_str);
            } else {
                std::string::const_iterator _aux = m_walker;
                ++_aux;
                const std::string _str(std::string(m_current, _aux));
                _type = m_tokens.recognize(_str);
            }

            if (_type != type::undefined) {
                // @note if \p m_tokens recognizes "=" and "==", and if the \p
                // m_walker is at the start of "== b", the first "=" will be
                // recognized, \p _ever_recognized will be set to \p true; but
                // then "==" is again recognized, making \p _ever_recognized to
                // be set again to \p true;
                _ever_recognized = true;
            }
            ++m_walker;
        }
    }

    if (_ever_recognized) {
        return symbol(lexeme(std::string(m_current, m_walker)), _type);
    }
    return symbol(lexeme(std::string(m_current, m_walker)), type::undefined);
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
