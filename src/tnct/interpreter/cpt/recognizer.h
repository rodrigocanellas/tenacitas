
#ifndef TNCT_INTERPRETER_CPT_RECOGNIZER_H
#define TNCT_INTERPRETER_CPT_RECOGNIZER_H

#include <optional>
#include <string>

#include "tnct/interpreter/dat/type.h"
#include "tnct/memory/cpt/has_new_operator.h"

namespace tnct::interpreter::cpt {

using recognizer_return =
    std::optional<std::pair<dat::type, std::string::const_iterator>>;

template <typename t>
concept recognizer =

    not std::copy_constructible<t> &&

    not std::copyable<t> &&

    std::move_constructible<t> &&

    not std::movable<t> &&

    not memory::cpt::has_new_operator_v<t> &&

    requires(t p_t, std::string::const_iterator p_begin,
             std::string::const_iterator p_end) {
      { p_t(p_begin, p_end) } -> std::same_as<recognizer_return>;
    };

} // namespace tnct::interpreter::cpt
#endif
