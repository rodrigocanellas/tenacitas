#ifndef TNCT_INTERPRETER_DAT_SYMBOL_H
#define TNCT_INTERPRETER_DAT_SYMBOL_H

#include <iostream>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

/// @brief Represents a symbol read from the text being interpreted
///
/// A symbol contains a lexema, which is the string read from the text being
/// analysed, and a type, like "integer", "variable_identifier" or
/// "reserved_word".
///
/// @tparam t_lexema_size is the maximum size of a lexema string
template <std::size_t t_lexema_size> class symbol_t final {

public:
  using lexema = lexema_t<t_lexema_size>;
  using lexema_reference = lexema_reference_t<t_lexema_size>;

  /// When the symbol is contructed by a terminal
  explicit symbol_t(lexema_reference p_lexema_reference,
                    type p_type = dat::unknow_type)
      : m_lexema_reference{p_lexema_reference}, m_type{p_type} {}

  /// When the symbol is contructed by a non terminal
  explicit symbol_t(std::string &&p_string, type p_type)
      : m_string{std::move(p_string)}, m_type{p_type} {}

  symbol_t() = delete;
  symbol_t(const symbol_t &) = default;
  symbol_t(symbol_t &&) = default;
  ~symbol_t() = default;

  symbol_t &operator=(const symbol_t &p_symbol) = default;
  symbol_t &operator=(symbol_t &&p_symbol) = default;

  bool is_defined() const {
    return m_lexema_reference.has_value() || m_string.has_value();
  }

  bool is_terminal() const { return m_lexema_reference.has_value(); }

  bool is_non_terminal() const { return m_string.has_value(); }

  std::optional<lexema_reference> get_lexema() const {
    return (m_lexema_reference ? m_lexema_reference : std::nullopt);
  }

  std::optional<std::reference_wrapper<const std::string>> get_value() const {
    if (m_string) {
      return {*m_string};
    }
    return std::nullopt;
  }

  type get_type() const { return m_type; };

  void set_type(type p_type) { m_type = p_type; };

  bool operator==(const symbol_t &p_symbol) const {
    if (m_type != p_symbol.m_type) {
      return false;
    }
    if (m_lexema_reference) {
      return (m_lexema_reference.get() == p_symbol.m_lexema_reference.get());
    }
    return (*m_string == *p_symbol.m_string);
  }

  bool operator!=(const symbol_t &p_symbol) const {
    return !(*this == p_symbol);
  };

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const symbol_t &p_symbol) {
    if (!p_symbol.is_defined()) {
      p_out << "()";
    } else {
      p_out << '(';
      if (p_symbol.is_terminal()) {
        p_out << p_symbol.get_lexema();
      } else {
        p_out << p_symbol.get_value();
      }
      p_out << ',' << p_symbol.get_type() << ')';
    }
    return p_out;
  }

private:
  std::optional<lexema_reference> m_lexema_reference;

  std::optional<std::string> m_string;

  type m_type;
};

} // namespace tnct::interpreter::dat

#endif
