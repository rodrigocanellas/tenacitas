#ifndef TENACITAS_INTERPRETER_BUSINESS_RECOGNIZER_H
#define TENACITAS_INTERPRETER_BUSINESS_RECOGNIZER_H

#include <functional>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief recognize function that tries to recognize a \p string according to a
/// pattern
///
typedef std::function<bool(const std::string&)> recognizer;

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif // RECOGNIZER_H
