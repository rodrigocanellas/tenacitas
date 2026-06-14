#ifndef TNCT_INTERPRETER_BUS_SCANNER_H
#define TNCT_INTERPRETER_BUS_SCANNER_H

#include <algorithm>
#include <string>

#include "tnct/interpreter/bus/recognizer.h"
#include "tnct/interpreter/bus/recognizers.h"
#include "tnct/interpreter/dat/column_number.h"
#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/line_number.h"
#include "tnct/interpreter/dat/symbol.h"
#include "tnct/interpreter/dat/terminal.h"
#include "tnct/interpreter/dat/terminals.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

/// Class responsible for retrieving tnct::interpreter::dat::symbol objects
/// from the text being analysed
///
///  This design tries to give a great flexibility to @p scanner, since  it can
///  adapt to any set of symbols to be recognized.
template <std::size_t t_lexema_size> class scanner final {

public:
  using lexema = dat::lexema_t<t_lexema_size>;
  using lexema_reference = dat::lexema_reference_t<t_lexema_size>;
  using terminal = dat::terminal_t<t_lexema_size>;
  using terminals = dat::terminals_t<t_lexema_size>;
  using symbol = dat::symbol_t<t_lexema_size>;

  ///
  /// @param [in] p_to_ignore is a string containing chars to be ignored while
  /// analysing the text
  scanner(std::initializer_list<char> p_to_ignore = {'\r', '\t', '\n'})
      : m_to_ignore(std::move(p_to_ignore)) {}

  scanner(const scanner &) = delete;
  scanner(scanner &&) = delete;

  ~scanner() = default;

  scanner &operator=(const scanner &) = delete;
  scanner &operator=(scanner &&) = delete;

  void *operator new(std::size_t) = delete;
  void *operator new[](std::size_t) = delete;

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

  void
  add_terminals(std::initializer_list<std::pair<std::string_view, dat::type>>
                    p_terminals) {
    m_terminals.import(p_terminals);
  }

  void add_terminal(std::string_view p_string, dat::type p_type) {
    m_terminals.add(p_string, p_type);
  }

  void import_terminals(const terminals &&p_terminals) {
    m_terminals.import(p_terminals);
  }

  void set_recognizers(std::initializer_list<recognizer> p_recognizers) {
    std::copy(p_recognizers.begin(), p_recognizers.end(),
              std::back_inserter(m_recognizers));
  }

  void add_recognizer(recognizer &&p_recognizer) {
    m_recognizers.push_back(std::move(p_recognizer));
  }

  void import_recognizers(const recognizers &p_recognizers) {
    std::copy(p_recognizers.begin(), p_recognizers.end(),
              std::back_inserter(m_recognizers));
  }

  /// Returns the next tnct::interpreter::dat::symbol object
  /// found in the text, which begin and end were defined using the @p
  /// set_text() method
  ///
  /// @return a tnct::interpreter::dat::symbol reference to the symbol
  /// read from the text
  ///
  /// @note If all the text has been read, the
  /// tnct::interpreter::dat::symbol_t has <tt> get_type ( ) ==
  /// tnct::interpreter::dat::terminal == tnct::interpreter::dat::end_of_text
  /// </tt>
  ///
  /// @throw tenacitas::number::fault with <tt> get_code ( ) ==
  /// error_unrecognized_symbol </tt> if no
  /// tnct::interpreter::terminal_recognizer, or
  /// tnct::interpreter::type_recognizer  was able to
  /// recognize a tnct::interpreter::symbol object in the
  /// analysed text
  std::optional<symbol> get_symbol() {
    skip();

    if (is_eot()) {
      return symbol{lexema_reference{lexema::end_of_text}, dat::end_of_text};
    }

    std::optional<symbol> _symbol{recognize_terminal()};
    if (_symbol) {
      return {std::move(_symbol)};
    }

    _symbol = recognize_non_terminal();
    if (_symbol) {
      return {std::move(_symbol)};
    }

    return std::nullopt;
  }

private:
  using terminal_recognition = dat::terminal_recognition_t<t_lexema_size>;

private:
  std::optional<symbol> recognize_non_terminal() {
    for (bus::recognizer &_recognizer : m_recognizers) {
      std::optional<dat::non_terminal_recognition> _res{
          _recognizer(m_walker, m_end)};

      if (_res && (_res->non_terminal.type != dat::unknow_type)) {
        m_walker = _res->end;
        return symbol(std::move(_res->non_terminal.value),
                      _res->non_terminal.type);
      }
    }
    return std::nullopt;
  }

  std::optional<symbol> recognize_terminal() {

    std::optional<terminal_recognition> _res{
        m_terminals.recognize(m_walker, m_end)};

    if (_res && _res->terminal.type != dat::unknow_type) {
      m_walker = _res->end;
      return symbol{_res->terminal.lexema_ref, _res->terminal.type};
    }

    return std::nullopt;
  }

  /// Advances the text indexers in order to ignore all the chars defined in
  /// the
  /// @p m_to_ignore
  void skip() {
    while (true) {

      if (is_eot() ||

          ((std::find(m_to_ignore.begin(), m_to_ignore.end(), *m_walker) ==
            m_to_ignore.end()) &&
           (*m_walker != ' '))) {
        break;
      }

      if (*m_walker == '\n') {
        ++m_curr_line;
        m_curr_col = 1;
      }

      ++m_curr_col;

      ++m_walker;
    }
  }

  /** @brief Informs if all the input text was analysed */
  bool is_eot() const { return ((m_walker == m_end) || (*m_walker == '\0')); }

  static const std::string m_fake_text;

  /// A string containing chars to be ignored while analysing the text
  std::vector<char> m_to_ignore;

  terminals m_terminals;

  bus::recognizers m_recognizers;

  dat::line_number m_curr_line{1};

  dat::column_number m_curr_col{1};

  std::string::const_iterator m_begin{m_fake_text.begin()};

  std::string::const_iterator m_end{m_fake_text.end()};

  std::string::const_iterator m_walker{m_end};

  dat::line_number m_line_last_symbol;

  dat::column_number m_col_last_symbol;
};

template <std::size_t t_lexema_size>
const std::string scanner<t_lexema_size>::m_fake_text{""};

} // namespace tnct::interpreter::bus

#endif
