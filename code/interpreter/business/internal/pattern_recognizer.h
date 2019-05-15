#ifndef TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZER_H
#define TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZER_H

#include <functional>
#include <list>
#include <string>

#include <interpreter/business/internal/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief recognize function that tries to recognize a \p type according to a
/// pattern
///
typedef std::function<type(const std::string&)> pattern_recognizer;

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif // RECOGNIZER_H
