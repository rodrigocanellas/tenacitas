
#ifndef TNCT_INTERPRETER_BUS_RECOGNIZERS_H
#define TNCT_INTERPRETER_BUS_RECOGNIZERS_H

#include <optional>
#include <set>
#include <string>
#include <utility>

#include "tnct/interpreter/bus/recognizer.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::bus {

using recognizers = std::set<recognizer>;

std::optional<std::pair<std::string::const_iterator, dat::type>>
recognize(const recognizers &p_recognizers, std::string::const_iterator p_begin,
          std::string::const_iterator p_end_of_text) {
  for (const recognizer &_recognizer : p_recognizers) {
    const auto _result{_recognizer(p_begin, p_end_of_text)};
    if (_result) {
      return _result;
    }
  }
  return std::nullopt;
}

void import(recognizers &p_recognizers, recognizers::const_iterator p_begin,
            recognizers::const_iterator p_end) {
  std::for_each(p_begin, p_end, [&](const recognizer &p_recognizer) {
    p_recognizers.emplace(p_recognizer);
  });
}

} // namespace tnct::interpreter::bus

#endif
