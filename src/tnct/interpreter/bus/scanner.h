#ifndef TNCT_INTERPRETER_BUS_SCANNER_H
#define TNCT_INTERPRETER_BUS_SCANNER_H

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "tnct/interpreter/bus/recognizer.h"
#include "tnct/interpreter/bus/recognizers.h"
#include "tnct/interpreter/dat/column_number.h"
#include "tnct/interpreter/dat/error_code.h"
#include "tnct/interpreter/dat/fault.h"
#include "tnct/interpreter/dat/line_number.h"
#include "tnct/interpreter/dat/symbol.h"
#include "tnct/interpreter/dat/token.h"
#include "tnct/interpreter/dat/tokens.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

/// Class responsible for retrieving tnct::interpreter::dat::symbol objects
/// from the text being analysed
///
///  This design tries to give a great flexibility to @p scanner, since  it can
///  adapt to any set of symbols to be recognized.
template <std::size_t t_lexema_size> class scanner final {

public:
  using token = dat::token_t<t_lexema_size>;
  using tokens = dat::terminals_t<t_lexema_size>;
  using symbol = dat::symbol_t<t_lexema_size>;

  ///
  /// @param [in] p_to_ignore is a string containing chars to be ignored while
  /// analysing the text
  scanner(std::string &&p_to_ignore = " \r\n\t")
      : m_to_ignore(std::move(p_to_ignore)) {}

  scanner(const std::string &p_to_ignore = " \r\n\t")
      : m_to_ignore(p_to_ignore) {}

  scanner(const scanner &) = delete;
  scanner(scanner &&) = delete;

  ~scanner();

  scanner &operator=(const scanner &) = delete;
  scanner &operator=(scanner &&) = delete;

  void set_text_to_scan(std::string::const_iterator p_begin,
                        std::string::const_iterator p_end) {
    m_begin = p_begin;
    m_end = p_end;
    m_walker = m_begin;
    m_curr_line = 1;
    m_curr_col = 1;
  }

  dat::line_number get_current_line() const { return m_curr_line; }

  dat::column_number get_current_column() const { return m_curr_col; };

  void add_tokens(std::initializer_list<token> p_tokens) {
    m_tokens.add(p_tokens);
  }
  void add_token(token &&p_token) { m_tokens.add({std::move(p_token)}); }
  void import_tokens(const tokens &&p_tokens) { m_tokens.add(p_tokens); }

  void set_recognizers(std::initializer_list<recognizer> p_recognizers) {
    std::for_each(p_recognizers.begin(), p_recognizers.end(),
                  [&](const recognizer &p_recognizer) {
                    m_recognizers.emplace(p_recognizer);
                  });
  }
  void add_recognizer(recognizer &&p_recognizer) {
    m_recognizers.emplace(p_recognizer);
  }
  void import_recognizers(const recognizers &p_recognizers) {
    std::for_each(p_recognizers.begin(), p_recognizers.end(),
                  [&](const recognizer &p_recognizer) {
                    m_recognizers.emplace(p_recognizer);
                  });
  }

  /// Returns the next tnct::interpreter::symbol object
  ///  found in the text, which begin and end were defined using the @p
  ///  set_text method
  ///
  /// @return a tnct::interpreter::symbol reference to the symbol
  /// read from the text
  ///
  /// @note If all the text has been read, the
  /// tnct::interpreter::dat::symbol_t has <tt> get_type ( ) ==
  /// tnct::interpreter::dat::token == tnct::interpreter::dat::end_of_text </tt>
  ///
  /// @throw tenacitas::number::fault with <tt> get_code ( ) ==
  /// error_unrecognized_symbol </tt> if no
  /// tnct::interpreter::token_recognizer, or
  /// tnct::interpreter::type_recognizer  was able to
  /// recognize a tnct::interpreter::symbol object in the
  /// analysed text
  std::optional<symbol> get_symbol() {
    skip();
    if (is_eot()) {
      return std::nullopt;
    }

    {
      using lexema_const_reference = typename tokens::lexema_const_reference;
      std::optional<std::pair<lexema_const_reference, dat::type>> _res{
          m_tokens.recognize(m_walker, m_end)};
      if (_res.has_value() && (_res->second != dat::end_of_text)) {
        return symbol(_res->first, _res->second);
      }
    }

    {
      std::optional<std::pair<std::string::const_iterator, dat::type>> _res{
          bus::recognize(m_recognizers, m_walker, m_end)};

      if (_res.has_value() && (_res->second != dat::end_of_text)) {
        return symbol({m_walker, _res->first}, _res->second);
      }
    }

    return std::nullopt;
  }

  //                         scanner
  /// @name                  Public Attributes

  /** @brief Indicates that no tnct::interpreter::symbol was
   recognized in the analysed text */
  static const dat::error_code error_unrecognized_symbol;

private:
  /// Advances the text indexers in order to ignore all the chars defined in the
  /// parameter @p p_to_ignore informed in the constructor
  void skip() {
    while (true) {

      if (*m_walker == '\n') {
        ++m_curr_line;
        m_curr_col = 1;
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

  /** @brief Informs if all the input text was analysed */
  bool is_eot() const { return ((m_walker == m_end) || (*m_walker == '\0')); }

  /** @brief Asks for each tnct::interpreter::recognizer object
   registered in a tnct::interpreter::recognizers to
   recognize a the tnct::interpreter::symbol from the input
   text

   @param [in] p_recognizers is the collection of @p recognizer
   objects used in the attempt to recognize a @o symbol from the
   input text

   @return a valif reference to a @ symbol, if a @p recognizer on
   @p p_recognizers was able to recognize a @p symbol

   @return a null reference, if <b> no </b> @p recognizer on @p
   p_recognizers was able to recognize a @p symbol */
  template <typename t_recognizer>
  std::shared_ptr<const symbol> recognize(const t_recognizer &p_recognizer);

  static const std::string m_fake_text;

  /// A string containing chars to be ignored while analysing the text
  const std::string m_to_ignore;

  tokens m_tokens;

  bus::recognizers m_recognizers;

  dat::line_number m_curr_line{1};

  dat::column_number m_curr_col{1};

  std::string::const_iterator m_begin{m_fake_text.begin()};

  std::string::const_iterator m_end{m_fake_text.end()};

  std::string::const_iterator m_walker{m_end};

  dat::line_number m_line_last_symbol;

  dat::column_number m_col_last_symbol;
};

} // namespace tnct::interpreter::bus

#endif
