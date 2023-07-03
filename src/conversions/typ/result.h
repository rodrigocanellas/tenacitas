#ifndef TENACITAS_LIB_CONVERSIONS_TYP_RESULT_H
#define TENACITAS_LIB_CONVERSIONS_TYP_RESULT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>
#include <string>
#include <utility>

namespace tenacitas::lib::src::conversions::typ {

template <typename t_to>
using result = std::pair<std::unique_ptr<std::string>, t_to>;

}

#endif
