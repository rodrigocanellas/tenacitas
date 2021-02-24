#ifndef TENACITAS_TYPE_H
#define TENACITAS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <type_traits>

/// \brief master namespace
namespace tenacitas {

/// \brief type traits
namespace type {

template <typename> struct is_tuple { const static bool value{false}; };

template <typename... T> struct is_tuple<std::tuple<T...>> {
  const static bool value{true};
};

} // namespace type

} // namespace tenacitas

#endif
