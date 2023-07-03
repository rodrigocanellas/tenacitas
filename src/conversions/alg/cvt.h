#ifndef TENACITAS_LIB_CONVERSIONS_ALG_CVT_H
#define TENACITAS_LIB_CONVERSIONS_ALG_CVT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <utility>

#include <tenacitas.lib/src/conversions/alg/int.h>

// namespace tenacitas::lib::src::conversions::alg {

///// \brief Basic type conversion template function
// template <typename t_to, typename t_from>
// constexpr std::pair<std::unique_ptr<std::string>, t_to> cvt(t_from &&) {
//   return {std::make_unique<std::string>("no conversion defined from '" +
//                                         std::string{typeid(t_from).name()} +
//                                         "' to '" +
//                                         std::string{typeid(t_to).name()} +
//                                         "'"),
//           {}};
// }

//} // namespace tenacitas::lib::src::conversions::alg

#endif
