#ifndef TNCT_INTERPRETER_DAT_LEXEMA_H
#define TNCT_INTERPRETER_DAT_LEXEMA_H

#include <iostream>
#include <string>
#include <string_view>

#include "tnct/string/dat/fixed_size_string.h"

namespace tnct::interpreter::dat {

/// String read from the input text.
///
/// @tparam t_size is the maximum size of a lexema
template <std::size_t t_size> class lexema_t final {
  static_assert(t_size > 6, "lexema_t size must be greater than 6");

public:
  using const_reference = std::reference_wrapper<const lexema_t>;

  explicit lexema_t(std::string_view p_string) : m_string(p_string) {}

  explicit lexema_t(std::string::const_iterator p_begin,
                    std::string::const_iterator p_end)
      : m_string({p_begin, p_end}) {}

  lexema_t() = delete;
  lexema_t(const lexema_t &) = default;
  lexema_t(lexema_t &&) = default;
  ~lexema_t() = default;

  lexema_t &operator=(const lexema_t &) = default;
  lexema_t &operator=(lexema_t &&) = default;

  operator std::string() const { return m_string; }

  bool operator==(const lexema_t &p_lexema_t) const {
    return m_string == p_lexema_t.m_string;
  }

  bool operator!=(const lexema_t &p_lexema_t) const {
    return m_string != p_lexema_t.m_string;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const lexema_t &p_lexema_t) {
    p_out << p_lexema_t.m_string;
    return p_out;
  }

  std::size_t size() const { return m_string.size(); }

  /// Very special lexema_t indicating that all the input text was analysed
  static const lexema_t eot;

private:
  string::dat::fixed_size_string<t_size> m_string;
};
template <std::size_t t_size>
const lexema_t<t_size> lexema_t<t_size>::eot{"!!eot!!"};

} // namespace tnct::interpreter::dat
#endif
