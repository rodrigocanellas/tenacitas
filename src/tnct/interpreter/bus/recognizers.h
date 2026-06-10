
#ifndef TNCT_INTERPRETER_BUS_RECOGNIZERS_H
#define TNCT_INTERPRETER_BUS_RECOGNIZERS_H

#include <algorithm>
#include <optional>
#include <set>
#include <string>
#include <utility>

#include "tnct/interpreter/cpt/recognizer.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

template <cpt::recognizer t_recognizer>
using recognizers = std::set<t_recognizer>;

template <cpt::recognizer t_recognizer>
std::optional<std::pair<std::string::const_iterator, dat::type>>
recognize(recognizers<t_recognizer> &p_recognizers,
          std::string::const_iterator p_begin,
          std::string::const_iterator p_end_of_text) {
  for (t_recognizer &_recognizer : p_recognizers) {
    const auto _result{_recognizer(p_begin, p_end_of_text)};
    if (_result) {
      return _result;
    }
  }
  return std::nullopt;
}

template <cpt::recognizer t_recognizer>
void import(recognizers<t_recognizer> &p_recognizers,
            typename recognizers<t_recognizer>::const_iterator p_begin,
            typename recognizers<t_recognizer>::const_iterator p_end) {
  std::for_each(p_begin, p_end, [&](t_recognizer &p_recognizer) {
    p_recognizers.emplace(p_recognizer);
  });
}

} // namespace tnct::interpreter::bus

#endif
