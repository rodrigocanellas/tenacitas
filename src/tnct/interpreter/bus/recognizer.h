
#ifndef TNCT_INTERPRETER_BUS_RECOGNIZER_H
#define TNCT_INTERPRETER_BUS_RECOGNIZER_H

#include <functional>
#include <optional>
#include <string>

#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

/// Tries to recognize the a dat::symbol from a std::string

class recognizer final {
public:
  /// Function that implements the recognition
  ///
  /// \param in a std::string::const_iterator where the string to be recognized
  /// begins
  /// \return {true/false, iterator-to-last-char-read}
  ///
  /// \note read tnct/interpreter/bus/standard_scanners.h for examples
  using function = std::function<std::pair<bool, std::string::const_iterator>(
      std::string::const_iterator p_begin)>;

  recognizer(dat::type p_type, function p_function)
      : m_type(p_type), m_function(p_function) {}

  recognizer() = delete;
  recognizer(const recognizer &) = delete;
  recognizer(recognizer &&) = default;
  ~recognizer() = default;

  recognizer &operator=(const recognizer &) = delete;
  recognizer &operator=(recognizer &&) = default;

  dat::type get_type() const { return m_type; }

  bool operator<(const recognizer &p_recognizer) const {
    return m_type < p_recognizer.get_type();
  }

  bool operator==(const recognizer &p_recognizer) const {
    return m_type == p_recognizer.get_type();
  }

  bool operator!=(const recognizer &p_recognizer) const {
    return m_type != p_recognizer.get_type();
  }

  std::optional<std::pair<std::string::const_iterator, dat::type>>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end_of_text) const {
    std::pair<bool, std::string::const_iterator> _recognized{
        m_function(p_begin)};
    if (_recognized.first) {
      return {{_recognized.second, m_type}};
    }
    if (_recognized.second == p_end_of_text) {
      return {{_recognized.second, dat::end_of_text}};
    }
    return std::nullopt;
  }

private:
  dat::type m_type;
  function m_function;
};

} // namespace tnct::interpreter::bus

#endif
