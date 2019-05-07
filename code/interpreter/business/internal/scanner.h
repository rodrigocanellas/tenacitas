#ifndef TENACITAS_INTERPRETER_BUSINESS_SCANNER__H
#define TENACITAS_INTERPRETER_BUSINESS_SCANNER__H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <std::string>

#include <interpreter/business/internal/symbol.h>
#include <interpreter/business/internal/type.h>

/// \brief Namespace of the organization
namespace tenacitas {
/// \brief Identifiers to allow the creation of a interpreter
namespace interpreter {

namespace business {

/// \brief Class responsible for retrieving
/// tenacitas::interpreter::symbol objects from the text being analysed
///
/// @p scanner uses tenacitas::interpreter::token_recognizers and
/// tenacitas::interpreter::type_recognizers objects to recognize
/// tenacitas::interpreter::symbol objects in the text.
///
/// This design tries to give a great flexibility to @p scanner, since
/// it can adapt to any set of symbols to be recognized.
class scanner
{

  public:
    /// \brief line is the line of the input text being scanned
    typedef uint32_t line;

    /// \brief column is the column of the input text being scanned
    typedef uint16_t column;

    /// \brief Constructor
    ///
    /// @param[in] p_to_ignore is a std::string containing chars to be ignored
    /// while analysing the text
    explicit scanner(const std::string& p_to_ignore = " \r\n\t");

    /// \brief Destructor
    ~scanner();

    /// \brief Defines the text to be analysed
    ///
    /// @param [in] p_begin refereces the first char in the text being analysed
    ///
    /// @param [in] p_end refereces the end of the text being analysed
    void set_text(std::string::const_iterator p_begin,
                  std::string::const_iterator p_end);

    /// \brief Retrieves the current line of the input text being analysed const
    line& get_line() const;

    /// \brief Retrieves the current column of the current line  of the  input
    /// text being analysed
    const column& get_column() const;

    /// \brief

    /// @param [in] p_tokens is the catalog of @p symbol objects that should be
    /// recognized by theirs @p symbol::value, direclty from the input text
    void set_tokens(const type& p_tokens);

    /// \brief

    @param[in] p_type_recognizers is the catalog of
      tenacitas::interpreter::type_recognizer objects void
      set_type_recognizers(
        memory::ptr<const type_recognizers> p_type_recognizers);

    /// @}                     ----------

    // -----------------------------------------------------------------
    //                         scanner
    /// @name                  Public Processors
    /// @{

    /// \brief Returns the next tenacitas::interpreter::symbol object
    found in the text,
      which begin and end were defined using the @p set_text method

      @ return a tenacitas::interpreter::symbol reference to the symbol read
        from the text

      @note If all the text has been read,
      the tenacitas::interpreter::symbol has<tt> get_value() ==
        tenacitas::interpreter::eot</ tt>

        memory::ptr<const symbol> get_symbol();

    /// @}                     ----------

    // -----------------------------------------------------------------
    //                         scanner
    /// @name                  Public Attributes
    /// @{

    /// \brief Indicates that no tenacitas::interpreter::symbol was
    recognized in the analysed
      text static const number::u16t error_unrecognized_symbol;

    /// @}                     ----------

  private:
    // -----------------------------------------------------------------
    //                         scanner
    /// @name                  Private Internal Types
    /// @{

    /// \brief Functor responsible for recognizing a
    tenacitas::interpreter::symbol from the input text,
      based on a collection of tenacitas::interpreter::symbol::value objects,
      defined in a tenacitas::interpreter::tokens object

      @nosubgrouping class token_recognizer
    {

      public:
        // ------------------------------------------------------------
        //                         token_recognizer
        /// @name                  Public Accessors
        /// @{

        /// \brief Defines the collection of
        tenacitas::interpreter::symbol::value that should be recognized from the
          input

          @param[in] p_tokens is a tenacitas::interpreter::tokens object that
            contains the collection of tenacitas::interpreter::tokens,
          that define the @p symbol::value object that should be recognized from
            the input text void
            set_tokens(memory::ptr<const tokens> p_tokens);

        /// @}                     ----------

        // ------------------------------------------------------------
        //                         token_recognizer
        /// @name                  Public Processors
        /// @{

        /// \brief Tries to recognize a tenacitas::interpreter::symbol
        from the input text,
          based on the tenacitas::interpreter::tokens object defined by the
          @p set_tokens method

          @param[in] p_end marks the end of the input text

          @param[in - out] p_walker is the current position in the input text
            memory::ptr<const symbol>
            recognize(std::string::const_iterator p_end,
                      std::string::const_iterator&p_walker) const;

        /// @}                     ----------

      private:
        // ------------------------------------------------------------
        //                         token_recognizer
        /// @name                  Private Attributes
        /// @{

        /// \brief The collection of
        tenacitas::interpreter::tokens that should be recognized from the input
          text memory::ptr<const tokens>
            m_tokens;

        /// @}                     ----------
    };

    /// @}                     ----------

    // -----------------------------------------------------------------
    //                         scanner
    /// @name                  Private Processors
    /// @{

    /// \brief Advances the text indexers in order to ignore all the
    chars defined in the parameter
      @p p_to_ignore informed in the constructor void
      skip();

    /// \brief Informs if all the input text was analysed
    bool is_eot() const;

    /// \brief Asks for each tenacitas::interpreter::recognizer object
    registered in a tenacitas::interpreter::recognizers to recognize a the
      tenacitas::interpreter::symbol from the input text

      @param[in] p_recognizers is the collection of
      @p recognizer objects used in the attempt to recognize a
      @o symbol from the input text

      @ return a valif reference to a @symbol,
      if a @p recognizer on @p p_recognizers was able to recognize a @p symbol

      @ return a null reference,
      if<b> no</ b> @p recognizer on @p p_recognizers was able to recognize a
      @p symbol template<typename t_recognizer>
      memory::ptr<const symbol> recognize(const t_recognizer&p_recognizer);

    /// @}                     ----------

    // -----------------------------------------------------------------
    //                         scanner
    /// @name                  Private Attributes
    /// @{

    /// \brief Catalog of tenacitas::interpreter::token_recognizer
    objects memory::ptr<const tokens> m_tokens;

    /// \brief Catalog of tenacitas::interpreter::type_recognizer
    objects memory::ptr<const type_recognizers> m_type_recognizers;

    /// \brief A std::string containing chars to be ignored while analysing
    the text const std::string m_to_ignore;

    /// \brief Current line in the text to be parsed
    line m_curr_line;

    /// \brief Current column in the current line
    column m_curr_col;

    /// \brief Begining of the text to be parsed
    std::string::const_iterator m_begin;

    /// \brief End of the text to be parsed
    std::string::const_iterator m_end;

    /// \brief Current position in the text to be parsed
    std::string::const_iterator m_walker;

    token_recognizer m_token_recognizer;

    /// \brief Row where the last symbol was recognized
    line m_line_last_symbol;

    /// \brief Col where the last symbol was recognized
    column m_col_last_symbol;

    /// @}                     ----------
};
} // namespace business
} // namespace interpreter
} // namespace tenacitas

// ====================================================================
// ====================================================================
// ====================================================================

//         I M P L E M E N T A T I O N

// ====================================================================
// ====================================================================
// ====================================================================

using namespace tenacitas::interpreter;

// ####################################################################
//                                    scanner

template<typename t_recognizer>
memory::ptr<const symbol>
scanner::recognize(const t_recognizer& p_recognizer)
{

    /// the resulting symbol
    memory::ptr<const symbol> rc;

    /// saving the current position being analysed in the text
    std::string::const_iterator l_walker = m_walker;

    rc = p_recognizer.recognize(m_end, l_walker);

    if (rc) {
        /// a tenacitas::interpreter::symbol was recognized, so we update the
        /// current position to be analysed in the text

        if (l_walker == m_walker) {
            /// however, if <tt> l_walker == m_walker </tt> this means that the
            /// @p symbol recognized is one byte long

            /// so, we need to move @p m_walker forward here
            ++m_walker;

            ++m_curr_col;

        } else {
            /// @p l_walker was "naturally" moved forward

            /// so, we update the column being analysed
            m_curr_col +=
              static_cast<column>(std::distance(m_walker, l_walker));

            /// and update @p m_walker
            m_walker = l_walker;
        }

        /// saving the row and column where the last symbol was recognized
        m_line_last_symbol = m_curr_line;
        m_col_last_symbol = m_curr_col;

    } else {
        /// no @p symbol was recognized, so we restore the position in the text,
        /// so that we try to recognize a @p symbol with another @p recognizer
        l_walker = m_walker;
    }

    return rc;
}

#endif
