#ifndef TENACITAS_LIB_TRAITS_ALG_PAIR_H
#define TENACITAS_LIB_TRAITS_ALG_PAIR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <utility>

/// \brief master namespace
namespace tenacitas::lib::traits::alg {

/// \brief Type trait that identifies if a type is not a \p pair
template <typename> struct is_pair { const static bool value{false}; };

/// \brief Type trait that identifies if a type is a tuple
template <typename... t> struct is_pair<std::pair<t...>> {
  const static bool value{true};
};

/// \brief Alias for a \p is_pair value
template <typename t> constexpr bool is_pair_v = is_pair<t>::value;

} // namespace tenacitas::lib::traits::alg

#endif // PAIR_H
