//#include "scanner.h"

// using namespace tenacitas::interpreter;

///// \brief Namespace of the organization
// namespace tenacitas {
///// \brief Identifiers to allow the creation of a interpreter
// namespace interpreter {

// namespace business {

// const uint16_t scanner::error_unrecognized_symbol(1);

////
///----------------------------------------------------------------------------
// scanner::scanner(std::string&& p_to_ignore)
//  : m_to_ignore(p_to_ignore)
//  , m_curr_line(1)
//  , m_curr_col(1)
//  , m_begin()
//  , m_end()
//  , m_walker()
//  , m_line_last_symbol(1)
//  , m_col_last_symbol(1)
//{}

////
///----------------------------------------------------------------------------
// void
// scanner::set_text(std::string::const_iterator p_begin,
//                  std::string::const_iterator p_end)
//{
//    m_curr_line = 1;
//    m_curr_col = 1;
//    m_begin = p_begin;
//    m_end = p_end;
//    m_walker = m_begin;
//    m_line_last_symbol = 1;
//    m_col_last_symbol = 1;
//}

////
///----------------------------------------------------------------------------
// void
// scanner::set_tokens(type&& p_tokens)
//{
//    m_tokens = std::move(p_tokens);
//}

////
///----------------------------------------------------------------------------
// symbol
// scanner::get_symbol()
//{

//    // before anything, let's skip the chars defined to be ignored
//    skip();

//    if (is_eot()) {
//        /// we reached the end of the text
//        rc = memory::ptr<symbol>(
//          new symbol(symbol::value::eot, symbol::type::eot));
//    } else {

//        /// trying to recognize by value
//        rc = recognize(m_token_recognizer);

//        if (rc == 0) {
//            /// trying to recognize by type
//            rc = recognize(*m_type_recognizers);
//        }
//    }

//    return rc;
//}

////
///----------------------------------------------------------------------------
// bool
// scanner::is_eot() const
//{
//    return ((m_walker == m_end) || (*m_walker == '\0'));
//}

////
///----------------------------------------------------------------------------
// void
// scanner::skip()
//{
//    while (true) {

//        if (*m_walker == '\n') {
//            ++m_curr_line;
//            m_curr_col = 1;
//            //             ++m_walker;
//            //             break;
//        }

//        if (m_to_ignore.find(*m_walker) == string::npos) {
//            break;
//        }

//        ++m_curr_col;

//        if (is_eot()) {
//            break;
//        }

//        ++m_walker;
//    }
//}

////
///----------------------------------------------------------------------------

// const line_number&
// scanner::get_line() const
//{
//    return m_line_last_symbol;
//}

////
///----------------------------------------------------------------------------
// const column_number&
// scanner::get_column() const
//{
//    return m_col_last_symbol;
//}

////
///############################################################################
////                                  scanner::token_recognizer

////
///----------------------------------------------------------------------------
// void
// scanner::token_recognizer::set_tokens(memory::ptr<const tokens> p_tokens)
//{
//    m_tokens = p_tokens;
//}

////
///----------------------------------------------------------------------------
// memory::ptr<const symbol>
// scanner::token_recognizer::recognize(string::const_iterator p_end,
//                                     string::const_iterator& p_walker) const
//{

//    memory::ptr<const symbol> rc;

//    if (m_tokens) {

//        bool recognized = false;

//        string::const_iterator l_walker = p_walker;

//        memory::ptr<const symbol> l_aux;

//        while (true) {

//            /// any of these conditions makes us stop
//            if ((p_walker == p_end) || (*p_walker == '\0') ||
//                (*p_walker == ' ') || (*p_walker == '\n'))
//                break;

//            /// if the @p symbol::value parsed  so far matches one of the
//            /// registered @p symbol::value that are to be recognized
//            if (std::distance(p_walker, l_walker) == 0) {
//                symbol::value l_symbol_value((string(1, *p_walker)));
//                l_aux = m_tokens->is_defined(l_symbol_value);
//            } else {
//                string::const_iterator aux = p_walker;
//                ++aux;
//                symbol::value l_symbol_value(string(l_walker, aux));
//                l_aux = m_tokens->is_defined(l_symbol_value);
//            }

//            if (l_aux != 0) {
//                /// @note if this @p token_recognizer recognizes "=" and "==",
//                /// and if the @p p_walker is referencing "== b", the first
//                "="
//                /// will be recognized, @p recognized will be set to @p true;
//                /// but then "==" is again recognized, making @p recognized to
//                /// be set again to @p true
//                rc = l_aux;
//                recognized = true;
//            }
//            //             else {
//            //                 /// the most recent @p symbol::value assembled
//            is
//            //                 not recognized break;
//            //             }

//            ++p_walker;
//        }
//    }

//    return rc;
//}

//} // namespace business
//} // namespace interpreter
//} // namespace tenacitas
