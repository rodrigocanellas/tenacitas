#ifndef TNCT_INTERPRETER_DAT_LEXEMA_H
#define TNCT_INTERPRETER_DAT_LEXEMA_H

#include <string_view>

#include "tnct/string/dat/fixed_size_string.h"

namespace tnct::interpreter::dat {

/// String read from the input text.
///
/// @tparam t_size is the maximum size of a lexema
///

// template <std::size_t t_size>
// using lexema_t = string::dat::fixed_size_string<t_size> ;

template <std::size_t t_size>
struct lexema_t : public string::dat::fixed_size_string<t_size> {
  static_assert(t_size >= 7, "size of lexema must be at least 7");

public:
  using const_reference = std::reference_wrapper<const lexema_t>;
  using const_iterator =
      typename string::dat::fixed_size_string<t_size>::const_iterator;

  explicit lexema_t(std::string_view p_string)
      : string::dat::fixed_size_string<t_size>(p_string) {}

  explicit lexema_t(const_iterator p_begin, const_iterator p_end)
      : string::dat::fixed_size_string<t_size>(p_begin, p_end) {}

  lexema_t() = delete;
  lexema_t(const lexema_t &) = default;
  lexema_t(lexema_t &&) = default;
  ~lexema_t() = default;

  lexema_t &operator=(const lexema_t &) = default;
  lexema_t &operator=(lexema_t &&) = default;

  /// Very special lexema_t indicating that all the input text was analysed
  static const lexema_t end_of_text;
};
template <std::size_t t_size>
const lexema_t<t_size> lexema_t<t_size>::end_of_text{"!!eot!!"};

} // namespace tnct::interpreter::dat
#endif
