#ifndef TENACITAS_INTERPRETER_BUSINESS_SCANNER_H
#define TENACITAS_INTERPRETER_BUSINESS_SCANNER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE
/// file the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <interpreter/business/internal/recognizers.h>
#include <interpreter/business/internal/symbol.h>
#include <interpreter/business/internal/tokens.h>
#include <interpreter/business/type.h>

/// \brief Namespace of the organization
namespace tenacitas {
/// \brief Identifiers to allow the creation of a interpreter
namespace interpreter {

namespace business {

/// \brief Class responsible for retrieving \p symbol objects from the text
/// being analysed
class scanner
{

  public:
    /// \brief line is the line of the input text being scanned
    typedef uint32_t line;

    /// \brief column is the column of the input text being scanned
    typedef uint16_t column;

    /// \brief Constructor
    ///
    /// \param[in] p_to_ignore is a std::string containing chars to be ignored
    /// while analysing the text
    explicit scanner(std::string&& p_to_ignore = " \r\n\t")
      : m_tokens()
      , m_to_ignore(p_to_ignore)
      , m_curr_line(1)
      , m_curr_col(1)
      , m_begin()
      , m_end()
      , m_current()
      , m_walker()
      , m_line_last_symbol(1)
      , m_col_last_symbol(1)
    {}

    /// \brief Destructor
    ~scanner() = default;

    /// \brief not allowed
    scanner(const scanner&) = delete;

    /// \brief default
    scanner(scanner&&) = default;

    /// \brief Defines the text to be analysed
    ///
    /// \param [in] p_begin refereces the first char in the text being analysed
    ///
    /// \param [in] p_end refereces the end of the text being analysed
    void set_text(std::string::const_iterator p_begin,
                  std::string::const_iterator p_end);

    /// \brief Retrieves the current line of the input text being analysed
    inline line get_line() const { return m_line_last_symbol; }

    /// \brief Retrieves the current column of the current line  of the  input
    /// text being analysed
    inline column get_column() const { return m_col_last_symbol; }

    /// \brief add_tokens adds a set of string values associated to a \p type
    /// \param p_type identifier of the group of tokens, like
    /// 'relational-operator'
    /// \param p_tokens the tokens, like '> < <= >='
    void add_tokens(const type& p_type, const std::string& p_tokens)
    {
        m_tokens.add(p_type, p_tokens);
    }

    /// \brief adds a \p recognizer
    /// \param p_recognizer is a new recognizer
    inline void add_recognizer(recognizer&& p_recognizer)
    {
        m_recognizers.add(std::move(p_recognizer));
    }

    /// \brief Returns the next \p:symbol object found in the text, which begin
    /// and end were defined using the @p set_text method
    ///
    /// \return a \p symbol reference to the symbol read from the
    /// text, or symbol::eot, which \p type is \p type::eot if all the text has
    /// been read
    symbol get_symbol();

    /// \brief not allowed
    scanner& operator=(const scanner&) = delete;

    /// \brief not allowed
    scanner& operator=(scanner&&) = delete;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

  private:
    /// \brief Advances the text indexers in order to ignore all the  chars
    /// defined in the parameter \p p_to_ignore informed in the constructor
    void skip();

    /// \brief Informs if all the input text was analysed
    inline bool is_eot() const
    {
        return ((m_current == m_end) || (*m_current == '\0'));
    }

    //    type recognize(recognizer p_recognizer);

    symbol recognize_by_token();
    symbol recognize_by_type();

  private:
    /// \brief set of tokens
    tokens m_tokens;

    /// \brief set og \p recognizer objects
    recognizers m_recognizers;

    /// \brief A std::string containing chars to be ignored while analysing the
    /// text
    std::string m_to_ignore;

    /// \brief Current line in the text to be parsed
    line m_curr_line;

    /// \brief Current column in the current line
    column m_curr_col;

    /// \brief Begining of the text to be parsed
    std::string::const_iterator m_begin;

    /// \brief End of the text to be parsed
    std::string::const_iterator m_end;

    /// \brief Current position in the text to be parsed
    std::string::const_iterator m_current;

    /// \brief possible next current possition
    std::string::const_iterator m_walker;

    /// \brief Row where the last symbol was recognized
    line m_line_last_symbol;

    /// \brief Col where the last symbol was recognized
    column m_col_last_symbol;
};
} // namespace business
} // namespace interpreter
} // namespace tenacitas

// using namespace tenacitas::interpreter;

//// ####################################################################
////                                    scanner

#endif
