/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_COMMON_H
#define TNCT_INTERPRETER_TST_COMMON_H

#include <cstddef>

#include "tnct/interpreter/bus/terminal_recognizer.h"
#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/symbol.h"
#include "tnct/interpreter/dat/terminal.h"
#include "tnct/interpreter/dat/terminal_recognition.h"
#include "tnct/interpreter/dat/terminals.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::tst {

constexpr std::size_t lexema_size{10};

using terminal = dat::terminal_t<lexema_size>;
using lexema = dat::lexema_t<lexema_size>;
using lexema_reference = dat::lexema_reference_t<lexema_size>;
using symbol = dat::symbol_t<lexema_size>;
using terminal_recognition = dat::terminal_recognition_t<lexema_size>;
using terminals = dat::terminals_t<lexema_size>;
using terminal_recognizer = bus::terminal_recognizer_t<lexema_size>;

constexpr dat::type word_type = 1;
constexpr dat::type integer_type = 2;
constexpr dat::type real_type = 3;
constexpr dat::type unary_operator = 4;
constexpr dat::type assignment_operator = 5;
constexpr dat::type reserved_word = 6;
constexpr dat::type expression_delimeter = 7;
constexpr dat::type comparision_operator = 8;

} // namespace tnct::interpreter::tst

#endif
