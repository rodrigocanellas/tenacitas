#ifndef TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZER_H
#define TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include <interpreter/business/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the group
namespace business {

/// \brief type of function that tries recognize a \p type from a string
typedef std::function<type(const std::string&)> recognizer;

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif // RECOGNIZER_H
