
#ifndef TNCT_INTERPRETER_CPT_RECOGNIZER_H
#define TNCT_INTERPRETER_CPT_RECOGNIZER_H

#include <optional>
#include <string>

#include "tnct/interpreter/dat/non_terminal_recognition.h"
#include "tnct/memory/cpt/has_new_operator.h"

namespace tnct::interpreter::cpt {

template <typename t>
concept recognizer =

    // std::copyable<t> &&

    // std::movable<t> &&

    // not memory::cpt::has_new_operator_v<t> &&

    requires(t p_t, std::string::const_iterator p_begin,
             std::string::const_iterator p_end) {
      {
        p_t(p_begin, p_end)
      } -> std::same_as<std::optional<dat::non_terminal_recognition>>;
    };

} // namespace tnct::interpreter::cpt
#endif
