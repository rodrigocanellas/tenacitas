/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_BUS_IS_DELIMETER_H
#define TNCT_INTERPRETER_BUS_IS_DELIMETER_H

#include <functional>
#include <string>

namespace tnct::interpreter::bus {

/// Determines if there is a delimeter char in a string
using is_delimeter = std::function<bool(std::string::const_iterator p_ite,
                                        std::string::const_iterator p_end)>;

} // namespace tnct::interpreter::bus

#endif
