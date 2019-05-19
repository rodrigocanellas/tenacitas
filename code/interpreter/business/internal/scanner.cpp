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
    if (_symbol.get_type() != type::undefined) {
        m_curr_col = static_cast<column>(std::distance(m_walker, m_current));
        m_current = ++m_walker;
        m_line_last_symbol = m_curr_line;
        m_col_last_symbol = m_curr_col;
        return _symbol;
    }

    return symbol(lexeme(""), type::undefined);
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
    return symbol(lexeme(""), type::undefined);
}

////----------------------------------------------------------------------------
// type
// scanner::recognize(recognizer p_recognizer)
//{

//    /// saving the current position being analysed in the text
//    std::string::const_iterator _walker = m_current;

//    type _type = p_recognizer(std::string(m_end, _walker));

//    if (_type == type::undefined) {
//        // no @p symbol was recognized, so we restore the position in the
//        text,
//        // so that we try to recognize a @p symbol with another @p recognizer

//        _walker = m_current;
//    } else {

//        // a \p type recognized, so we update the current position to be
//        // analysed in the text

//        if (_walker == m_current) {
//            // however, if <tt> _walker == m_current </tt> this means that the
//            // @p symbol recognized is one byte long

//            // so, we need to move @p m_current forward here
//            ++m_current;

//            ++m_curr_col;

//        } else {
//            // @p _walker was "naturally" moved forward

//            // so, we update the column being analysed
//            m_curr_col += static_cast<column>(std::distance(m_current,
//            _walker));

//            // and update @p m_current
//            m_current = _walker;
//        }

//        /// saving the row and column where the last symbol was recognized
//        m_line_last_symbol = m_curr_line;
//        m_col_last_symbol = m_curr_col;
//    }
//    return _type;
//}

// ----------------------------------------------------------------------------
symbol
scanner::recognize_by_token()
{

    type _type(type::undefined);
    m_walker = m_current;
    bool _ever_recognized = false;

    if (m_tokens.empty()) {

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
                const std::string _str(std::string(m_walker, _aux));
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
    return symbol(lexeme(""), type::undefined);
}

} // namespace business
} // namespace interpreter
} // namespace tenacitas
